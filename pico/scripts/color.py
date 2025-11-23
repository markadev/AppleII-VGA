import itertools
import numpy as np

from math import pi, radians
from scipy.signal import butter, iircomb, lfilter, sosfilt


Fc = (315 / 88) * 1000000  # NTSC color carrier frequency
RESAMPLE_FACTOR = 16  # Expand each screen bit to this number of samples for finer interpolation
SAMPLES_PER_COLORCYCLE = 4 * RESAMPLE_FACTOR
SAMPLE_RATE = Fc * SAMPLES_PER_COLORCYCLE


NTSC53_YIQ_TO_RGB = np.array([
    [ 1.0, 0.956, 0.619 ],
    [ 1.0, -0.272, -0.647 ],
    [ 1.0, -1.106, 1.703 ]
])

NTSC53_RGB_TO_YIQ = np.array([
    [ 0.299, 0.587, 0.114 ],
    [ 0.5959, -0.2746, -0.3213 ],
    [ 0.2115, -0.5227, 0.3112 ]
])


def upsample(arr):
    # abc -> aa...bb...cc...
    return np.repeat(arr, RESAMPLE_FACTOR)

def downsample(arr):
    # abcd... -> avg(ab), avg(cd),...
    return np.average(arr.reshape(-1, RESAMPLE_FACTOR), axis=1)

def lowpass_filter(data, cutoff, order=5):
    sos = butter(order, cutoff, fs=SAMPLE_RATE, btype='lowpass', output='sos')
    return sosfilt(sos, data)

def highpass_filter(data, cutoff, order=5):
    sos = butter(order, cutoff, fs=SAMPLE_RATE, btype='highpass', output='sos')
    return sosfilt(sos, data)

def bandpass_filter(data, center, width, order=5):
    lowcut = center - (width / 2)
    highcut = center + (width / 2)
    sos = butter(order, [lowcut, highcut], fs=SAMPLE_RATE, btype='band', output='sos')
    return sosfilt(sos, data)

def notch_filter(data, center, width, order=5):
    Q = center / width
    b, a = iircomb(center, Q, fs=SAMPLE_RATE, ftype='notch')
    return lfilter(b, a, data)

def scale_to_int(arr, upper_bound):
    """Uniformly scale an array from the range [0.0, 1.0] to [0, int(upper_bound) )"""
    # Uniform range transformation: [0.0, 1.0] -> [0.0, upper_bound] -> floor -> [0, int(upper_bound)] -> clip -> [0, int(upper_bound))
    return np.floor(arr * float(upper_bound)).astype(int).clip(0, int(upper_bound) - 1)



class AnalogColorDecoder:
    def __init__(self, luma_bandlimit=2_300_000, luma_lag=0,
                 chroma_bandwidth=2_400_00, chroma_lag=0,
                 color_adjustment=0.0, color_saturation=1.0, contrast=1.0):
        self.luma_bandlimit = luma_bandlimit
        self.luma_lag = int(luma_lag)
        self.chroma_bandwidth = chroma_bandwidth
        self.chroma_lag = int(chroma_lag)

        self.color_adjustment = float(color_adjustment)
        self.color_saturation = float(color_saturation)
        self.contrast = float(contrast)

    def decode(self, sig, color_phase=0.0):
        # Resample the incoming signal at a higher rate for smoother filtering
        sig = upsample(sig)

        luma = np.append(sig, np.zeros(self.luma_lag))  # pad at the end so the beginning can be chopped off
        luma = lowpass_filter(luma, self.luma_bandlimit, order=7)
        luma = luma[self.luma_lag:]
        luma = luma.clip(0.0, 1.0)

        chroma = np.append(sig, np.zeros(self.chroma_lag + self.luma_lag))  # pad at the end so the beginning can be chopped off
        chroma = bandpass_filter(chroma, Fc, self.chroma_bandwidth, order=3)
        rads = np.tile(np.linspace(0, 2*pi, SAMPLES_PER_COLORCYCLE, False), int(len(chroma) / SAMPLES_PER_COLORCYCLE) + 1)[:len(chroma)]
        rads += radians(self.color_adjustment + color_phase - 18)  # align the color reference to the delayed chroma signal
        ref_sin = np.sin(rads)
        ref_cos = np.cos(rads)

        i = lowpass_filter(chroma * ref_cos, Fc).clip(-0.5957, 0.5957)
        q = lowpass_filter(chroma * ref_sin, Fc).clip(-0.5226, 0.5226)
        # shift the chroma signal back in phase with the luma signal
        i = i[self.chroma_lag + self.luma_lag:]
        q = q[self.chroma_lag + self.luma_lag:]

        i *= self.color_saturation
        q *= self.color_saturation
        luma *= self.contrast

        # downsample back to the original number of samples
        return np.array([downsample(luma), downsample(i), downsample(q)])


class DiscreteColorDecoder:
    def __init__(self, primary_rgb8_colors, smooth=False):
        colors = np.array(primary_rgb8_colors)
        assert colors.shape == (16, 3)

        self.yiq_colors = np.matmul(NTSC53_RGB_TO_YIQ, (colors / 255).transpose()).transpose()
        self.smooth = smooth

    def decode_one(self, dots, color_phase_deg=0):
        # dots - An array of at least 7 samples, where the 4th is the dot being rendered and previous samples
        # are previously rendered dots
        #
        #  0  1  2  3  4  5  6 ....
        #  \_____/  ^  \_____/
        #   past    |  future
        #   dots    |   dots
        #        current
        #          dot
        assert dots.shape[0] >= 7

        # adjust phase so the color reference aligns at bit 3 instead of bit 0
        color_phase_deg -= (3 * 90)

        dots = dots[0:7].astype(int)

        r = int(color_phase_deg // 90)
        bit_patterns = [
            np.packbits(np.roll(dots[0:4], r), bitorder='little')[0],
            np.packbits(np.roll(dots[1:5], r+1), bitorder='little')[0],
            np.packbits(np.roll(dots[2:6], r+2), bitorder='little')[0],
            np.packbits(np.roll(dots[3:7], r+3), bitorder='little')[0],
        ]
        #bits = np.packbits(dots, bitorder='little')[0]
        #print(f"{bits:07b} {color_phase_deg} {bit_patterns[0]:04b} {bit_patterns[1]:04b} {bit_patterns[2]:04b} {bit_patterns[3]:04b}")

        if not self.smooth:
            # Hard transition from full-black and full-white
            if 0x0 in bit_patterns:
                return self.yiq_colors[0]
            elif 0xf in bit_patterns:
                return self.yiq_colors[15]

        # Use a weighted average of the 4 colors described by the 4 bit patterns
        yiqs = np.array([ self.yiq_colors[i] for i in bit_patterns ]).transpose()
        return np.average(yiqs, weights=[0.2, 1.0, 0.5, 0.2], axis=1)


class QuantizedYIQPalette:
    """Convert to RGB colorspace using the closest color in YIQ coordinate-space"""
    def __init__(self, weights=None, rgb8_colors=None):
        self.weights = weights if weights else [1.0, 1.0, 1.0]

        # Pregenerate the RGB and corresponding YIQ color values
        if rgb8_colors:
            self.rgb = np.array(rgb8_colors) / 255
        else:
            self.rgb = np.array(list(itertools.product(range(8), range(8), range(8)))) / 7
        self.yiq = np.matmul(NTSC53_RGB_TO_YIQ, self.rgb.transpose())

    def rgb_to_rgb(self, rgb):
        yiq = np.matmul(NTSC53_RGB_TO_YIQ, rgb.transpose())
        return self.yiq_to_rgb(yiq)

    def yiq_to_rgb(self, yiq):
        assert yiq.shape[0] == 3

        y, i, q = yiq

        y = np.array([y]).transpose()
        i = np.array([i]).transpose()
        q = np.array([q]).transpose()

        # dy[n] = abs(y - yiq[n][0])
        dy = np.abs(y - self.yiq[0]) * self.weights[0]

        # di[n] = abs(i - yiq[n][1])
        di = np.abs(i - self.yiq[1]) / (2*0.5957) * self.weights[1]

        # dq[n] = abs(q - yiq[n][2])
        dq = np.abs(q - self.yiq[2]) / (2*0.5226) * self.weights[2]

        distance_sq = dy*dy + di*di + dq*dq

        # Get index of the smallest element (least distance)
        i = np.argpartition(distance_sq, 1).transpose()[:1][0]
        return self.rgb[i]


class FullRGBPalette:
    """Convert to RGB colorspace in full 24 bit color"""
    def rgb_to_rgb(self, rgb):
        return rgb

    def yiq_to_rgb(self, yiq):
        assert yiq.shape[0] == 3
        return np.matmul(NTSC53_YIQ_TO_RGB, yiq).clip(0.0, 1.0).transpose()


class QuantizedRGBPalette(FullRGBPalette):
    """Convert to RGB colorspace, quantized to 3 bits per color"""
    def rgb_to_rgb(self, rgb):
        return np.floor(rgb * 8).clip(0, 7) / 8

    def yiq_to_rgb(self, yiq):
        return np.floor(super().yiq_to_rgb(yiq) * 8).clip(0, 7) / 8
