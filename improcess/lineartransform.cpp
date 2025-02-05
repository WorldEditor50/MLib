#include "lineartransform.h"

int ns::linearTransform(OutTensor xo, InTensor xi, float alpha, float beta)
{
    xo = Tensor(xi.shape);
    for (std::size_t i = 0; i < xi.totalSize; i++) {
        xo.val[i] = clip(xi.val[i]*alpha + beta, 0, 255);
    }
    return 0;
}

int ns::logTransform(OutTensor xo, InTensor xi, float c)
{
    xo = Tensor(xi.shape);
    for (std::size_t i = 0; i < xi.totalSize; i++) {
        xo.val[i] = clip(c*std::log(xi.val[i] + 1), 0, 255);
    }
    return 0;
}

int ns::gammaTransform(OutTensor xo, InTensor xi, float esp, float gamma)
{
    xo = Tensor(xi.shape);
    for (std::size_t i = 0; i < xi.totalSize; i++) {
        float p = std::pow((xi.val[i] + esp)/255.0, gamma)*255;
        xo.val[i] = clip(p, 0, 255);
    }
    return 0;
}

int ns::histogramEqualize(OutTensor xo, InTensor xi)
{
    if (xi.shape[HWC_C] != 1) {
        return -1;
    }
    /*
        PDF : ∫p(x)dx=1

        origin image PDF: pr
        equalize image PDF: ps, ∫ps(x)dx=1
        ps * ds = ps * ds

        CDF: s = ∫pr(x)dx

    */
    xo = Tensor(xi.shape);
    /* 1. histogram */
    Tensor hist;
    uniformHistogram(hist, xi);
    /* 2. equalize */
    for (std::size_t i = 0; i < xi.totalSize; i++) {
        float cdf = 0;
        for (std::size_t j = 0; j < xi.val[i]; j++) {
            cdf += hist.val[j];
        }
        xo.val[i] = clip(cdf*255.0, 0, 255);
    }
    return 0;
}

int ns::histogramStandardize(OutTensor xo, InTensor xi)
{
    if (xi.shape[HWC_C] != 1) {
        return -1;
    }
    xo = Tensor(xi.shape);
    /* histogram */
    Tensor hist;
    uniformHistogram(hist, xi);

    /* equalize */
    int histStd[256] = {-1};
    for (int i = 0; i < 256; i++) {
        float s = 0;
        for (int j = 0; j < i; j++) {
            s += hist[j];
        }
        histStd[int(0.5 + 255*s)] = i;
    }
    /* interpolation */
    for (int i = 0; i < 255; i++) {
        if (histStd[i + 1] != -1) {
            continue;
        }
        for (int j = 1; i + j < 255; j++) {
            if (histStd[i + j] == -1) {
                continue;
            }
            histStd[i + j] = histStd[i];
        }
    }
    /* standardize */
    for (std::size_t i = 0; i < xi.totalSize; i++) {
        float cdf = 0;
        for (std::size_t j = 0; j < xi.val[i]; j++) {
            cdf += hist[j];
        }
        xo.val[i] = clip(histStd[int(255*cdf)], 0, 255);
    }
    return 0;
}
