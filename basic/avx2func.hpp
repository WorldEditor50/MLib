#ifndef AVX2FUNC_HPP
#define AVX2FUNC_HPP
#include <immintrin.h>
#include <vector>
#include <cmath>
namespace simd {

struct AVX2 {

#if defined(__AVX2__)

    template<typename T>
    struct Step {
        constexpr static std::size_t value = 0;
    };
    template<>
    struct Step<double> {
        constexpr static std::size_t value = sizeof (__m256d)/sizeof (double);
    };
    template<>
    struct Step<float> {
        constexpr static std::size_t value = sizeof (__m256)/sizeof (float);
    };

    inline static float reduce(__m256& ymm)
    {
        float result[8];
        ymm = _mm256_hadd_ps(ymm, ymm);
        ymm = _mm256_hadd_ps(ymm, ymm);
        _mm256_storeu_ps(result, ymm);
        return result[0] + result[4];
    }

    inline static double reduce(__m256d& ymm)
    {
        double result[4];
        ymm = _mm256_hadd_pd(ymm, ymm);
        _mm256_storeu_pd(result, ymm);
        return result[0] + result[2];
    }

    inline static void fill(double* __restrict x, double x0, std::size_t N)
    {
        double* px = x;
        std::size_t r = N%4;
        __m256d vecx0 = _mm256_setr_pd(x0, x0, x0, x0);
        for (std::size_t i = 0; i < N - r; i+=4) {
            _mm256_store_pd(px + i, vecx0);
        }
        for (std::size_t i = N - r; i < N; i++) {
            x[i] = x0;
        }
        return;
    }

    inline static void fill(float* __restrict x, float x0, std::size_t N)
    {
        float* px = x;
        std::size_t r = N%8;
        __m256 vecx0 = _mm256_setr_ps(x0, x0, x0, x0, x0, x0, x0, x0);
        for (std::size_t i = 0; i < N - r; i+=8) {
            _mm256_store_ps(px + i, vecx0);
        }
        for (std::size_t i = N - r; i < N; i++) {
            x[i] = x0;
        }
        return;
    }

    inline static void add(double* __restrict z, const double* __restrict y, const double* __restrict x, std::size_t N)
    {
        const double *px = x;
        const double *py = y;
        double *pz = z;
        /* __m256d: AVX double */
        __m256d vecx;
        __m256d vecy;
        __m256d vecz;
        /* step */
        std::size_t step = sizeof (__m256d)/sizeof (double);
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            /* put 4 double into __m256d */
            vecx = _mm256_loadu_pd(px + i);
            vecy = _mm256_loadu_pd(py + i);
            /* add */
            vecz = _mm256_add_pd(vecx, vecy);
            /* store result */
            _mm256_storeu_pd(pz + i, vecz);
        }
        return;
    }

    inline static void sub(double* __restrict z, const double* __restrict y, const double* __restrict x, std::size_t N)
    {
        const double *px = x;
        const double *py = y;
        double *pz = z;
        /* __m256d: AVX double */
        __m256d vecx;
        __m256d vecy;
        __m256d vecz;
        /* offset */
        /* step */
        std::size_t step = sizeof (__m256d)/sizeof (double);
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            /* put 4 double into __m256d */
            vecx = _mm256_loadu_pd(px + i);
            vecy = _mm256_loadu_pd(py + i);
            /* add */
            vecz = _mm256_sub_pd(vecx, vecy);
            /* store result */
            _mm256_storeu_pd(pz + i, vecz);
        }
        return;
    }

    inline static void mul(double* __restrict z, const double* __restrict y, const double* __restrict x, std::size_t N)
    {
        const double *px = x;
        const double *py = y;
        double *pz = z;
        /* __m256d: AVX double */
        __m256d vecx;
        __m256d vecy;
        __m256d vecz;
        /* step */
        std::size_t step = sizeof (__m256d)/sizeof (double);
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            /* put 4 double into __m256d */
            vecx = _mm256_loadu_pd(px + i);
            vecy = _mm256_loadu_pd(py + i);
            /* add */
            vecz = _mm256_mul_pd(vecx, vecy);
            /* store result */
            _mm256_storeu_pd(pz + i, vecz);
        }
        return;
    }

    inline static void div(double* __restrict z, const double* __restrict y, const double* __restrict x, std::size_t N)
    {
        const double *px = x;
        const double *py = y;
        double *pz = z;
        /* __m256d: AVX double */
        __m256d vecx;
        __m256d vecy;
        __m256d vecz;
        /* step */
        std::size_t step = sizeof (__m256d)/sizeof (double);
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            /* put 4 double into __m256d */
            vecx = _mm256_loadu_pd(px + i);
            vecy = _mm256_loadu_pd(py + i);
            /* add */
            vecz = _mm256_div_pd(vecx, vecy);
            /* store result */
            _mm256_storeu_pd(pz + i, vecz);
        }
        return;
    }

    inline static void add(float* __restrict z, const float* __restrict y, const float* __restrict x, std::size_t N)
    {
        const float *px = x;
        const float *py = y;
        float *pz = z;
        /* __m256: AVX2 float */
        __m256 vecx;
        __m256 vecy;
        __m256 vecz;
        /* step */
        std::size_t step = sizeof (__m256)/sizeof (float);
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            /* put 4 double into __m256d */
            vecx = _mm256_loadu_ps(px + i);
            vecy = _mm256_loadu_ps(py + i);
            /* add */
            vecz = _mm256_add_ps(vecx, vecy);
            /* store result */
            _mm256_storeu_ps(pz + i, vecz);
        }
        return;
    }

    inline static void sub(float* __restrict z, const float* __restrict y, const float* __restrict x, std::size_t N)
    {
        const float *px = x;
        const float *py = y;
        float *pz = z;
        /* __m256: AVX2 float */
        __m256 vecx;
        __m256 vecy;
        __m256 vecz;
        /* step */
        std::size_t step = sizeof (__m256)/sizeof (float);
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            /* put 4 double into __m256d */
            vecx = _mm256_loadu_ps(px + i);
            vecy = _mm256_loadu_ps(py + i);
            /* add */
            vecz = _mm256_sub_ps(vecx, vecy);
            /* store result */
            _mm256_storeu_ps(pz + i, vecz);
        }
        return;
    }

    inline static void mul(float* __restrict z, const float* __restrict y, const float* __restrict x, std::size_t N)
    {
        const float *px = x;
        const float *py = y;
        float *pz = z;
        /* __m256: AVX2 float */
        __m256 vecx;
        __m256 vecy;
        __m256 vecz;
        /* step */
        std::size_t step = sizeof (__m256)/sizeof (float);
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            /* put 4 double into __m256d */
            vecx = _mm256_loadu_ps(px + i);
            vecy = _mm256_loadu_ps(py + i);
            /* add */
            vecz = _mm256_mul_ps(vecx, vecy);
            /* store result */
            _mm256_storeu_ps(pz + i, vecz);
        }
        return;
    }

    inline static void div(float* __restrict z, const float* __restrict y, const float* __restrict x, std::size_t N)
    {
        const float *px = x;
        const float *py = y;
        float *pz = z;
        /* __m256: AVX2 float */
        __m256 vecx;
        __m256 vecy;
        __m256 vecz;
        /* step */
        std::size_t step = sizeof (__m256)/sizeof (float);
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            /* put 4 double into __m256d */
            vecx = _mm256_loadu_ps(px + i);
            vecy = _mm256_loadu_ps(py + i);
            /* add */
            vecz = _mm256_div_ps(vecx, vecy);
            /* store result */
            _mm256_storeu_ps(pz + i, vecz);
        }
        return;
    }

    inline static void add(double* __restrict z, const double* __restrict y, double x, std::size_t N)
    {
        const double *py = y;
        double *pz = z;
        __m256d vecx = _mm256_setr_pd(x, x, x, x);
        __m256d vecy;
        __m256d vecz;
        std::size_t step = 4;
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            vecy = _mm256_loadu_pd(py + i);
            vecz = _mm256_add_pd(vecy, vecx);
            _mm256_storeu_pd(pz + i, vecz);
        }
        for (std::size_t i = N - r; i < N; i++) {
            z[i] = y[i] + x;
        }
        return;
    }

    inline static void sub(double* __restrict z, const double* __restrict y, double x, std::size_t N)
    {
        const double *py = y;
        double *pz = z;
        __m256d vecx = _mm256_setr_pd(x, x, x, x);
        __m256d vecy;
        __m256d vecz;
        std::size_t step = 4;
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            vecy = _mm256_loadu_pd(py + i);
            vecz = _mm256_sub_pd(vecy, vecx);
            _mm256_storeu_pd(pz + i, vecz);
        }
        for (std::size_t i = N - r; i < N; i++) {
            z[i] = y[i] - x;
        }
        return;
    }

    inline static void mul(double* __restrict z, const double* __restrict y, double x, std::size_t N)
    {
        const double *py = y;
        double *pz = z;
        __m256d vecx = _mm256_setr_pd(x, x, x, x);
        __m256d vecy;
        __m256d vecz;
        std::size_t step = 4;
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            vecy = _mm256_loadu_pd(py + i);
            vecz = _mm256_mul_pd(vecy, vecx);
            _mm256_storeu_pd(pz + i, vecz);
        }
        for (std::size_t i = N - r; i < N; i++) {
            z[i] = y[i] * x;
        }
        return;
    }

    inline static void div(double* __restrict z, const double* __restrict y, double x, std::size_t N)
    {
        const double *py = y;
        double *pz = z;
        __m256d vecx = _mm256_setr_pd(x, x, x, x);
        __m256d vecy;
        __m256d vecz;
        std::size_t step = 4;
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            vecy = _mm256_loadu_pd(py + i);
            vecz = _mm256_sub_pd(vecy, vecx);
            _mm256_storeu_pd(pz + i, vecz);
        }
        for (std::size_t i = N - r; i < N; i++) {
            z[i] = y[i] / x;
        }
        return;
    }

    inline static void add(float* __restrict z, const float* __restrict y, float x, std::size_t N)
    {
        const float *py = y;
        float *pz = z;
        __m256 vecx = _mm256_setr_ps(x, x, x, x, x, x, x, x);
        __m256 vecy;
        __m256 vecz;
        std::size_t step = 8;
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            vecy = _mm256_loadu_ps(py + i);
            vecz = _mm256_add_ps(vecy, vecx);
            _mm256_storeu_ps(pz + i, vecz);
        }
        for (std::size_t i = N - r; i < N; i++) {
            z[i] = y[i] + x;
        }
        return;
    }

    inline static void sub(float* __restrict z, const float* __restrict y, float x, std::size_t N)
    {
        const float *py = y;
        float *pz = z;
        __m256 vecx = _mm256_setr_ps(x, x, x, x, x, x, x, x);
        __m256 vecy;
        __m256 vecz;
        std::size_t step = 8;
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            vecy = _mm256_loadu_ps(py + i);
            vecz = _mm256_sub_ps(vecy, vecx);
            _mm256_storeu_ps(pz + i, vecz);
        }
        for (std::size_t i = N - r; i < N; i++) {
            z[i] = y[i] - x;
        }
        return;
    }

    inline static void mul(float* __restrict z, const float* __restrict y, float x, std::size_t N)
    {
        const float *py = y;
        float *pz = z;
        __m256 vecx = _mm256_setr_ps(x, x, x, x, x, x, x, x);
        __m256 vecy;
        __m256 vecz;
        std::size_t step = 8;
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            vecy = _mm256_loadu_ps(py + i);
            vecz = _mm256_mul_ps(vecy, vecx);
            _mm256_storeu_ps(pz + i, vecz);
        }
        for (std::size_t i = N - r; i < N; i++) {
            z[i] = y[i] * x;
        }
        return;
    }

    inline static void div(float* __restrict z, const float* __restrict y, float x, std::size_t N)
    {
        const float *py = y;
        float *pz = z;
        __m256 vecx = _mm256_setr_ps(x, x, x, x, x, x, x, x);
        __m256 vecy;
        __m256 vecz;
        std::size_t step = 8;
        std::size_t r = N%step;
        for (std::size_t i = 0; i < N - r; i+= step) {
            vecy = _mm256_loadu_ps(py + i);
            vecz = _mm256_div_ps(vecy, vecx);
            _mm256_storeu_ps(pz + i, vecz);
        }
        for (std::size_t i = N - r; i < N; i++) {
            z[i] = y[i] / x;
        }
        return;
    }


    inline static double max(const double* __restrict x, std::size_t N)
    {
        const double *px = x;
        /* align */
        std::size_t r = N%4;
        /* find max value per 8 element */
        __m256d maxVec = _mm256_loadu_pd(px);
        for (std::size_t i = 4; i < N - r; i+=4) {
            __m256d vecx = _mm256_loadu_pd(px + i);
            maxVec = _mm256_max_pd(vecx, maxVec);
        }
        /* find max value in result */
        double result[4];
        _mm256_storeu_pd(result, maxVec);
        double maxValue = result[0];
        for (std::size_t i = 1; i < 4; i++) {
            maxValue = result[i] > maxValue ? result[i] : maxValue;
        }
        /* find max value in the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            maxValue = px[i] > maxValue ? px[i] : maxValue;
        }
        return maxValue;
    }

    inline static float max(const float* __restrict x, std::size_t N)
    {
        const float *px = x;
        /* align */
        std::size_t r = N%8;
        /* find max value per 8 element */
        __m256 maxVec = _mm256_loadu_ps(px);
        for (std::size_t i = 8; i < N - r; i+=8) {
            __m256 vecx = _mm256_loadu_ps(px + i);
            maxVec = _mm256_max_ps(vecx, maxVec);
        }
        /* find max value in result */
        float result[8];
        _mm256_storeu_ps(result, maxVec);
        float maxValue = result[0];
        for (std::size_t i = 1; i < 8; i++) {
            maxValue = result[i] > maxValue ? result[i] : maxValue;
        }
        /* find max value in the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            maxValue = px[i] > maxValue ? px[i] : maxValue;
        }
        return maxValue;
    }


    inline static double min(const double* __restrict x, std::size_t N)
    {
        const double *px = x;
        /* align */
        std::size_t r = N%4;
        /* find min value per 8 element */
        __m256d minVec = _mm256_loadu_pd(px);
        for (std::size_t i = 4; i < N - r; i+=4) {
            __m256d vecx = _mm256_loadu_pd(px + i);
            minVec = _mm256_min_pd(vecx, minVec);
        }
        /* find min value in result */
        double result[4];
        _mm256_storeu_pd(result, minVec);
        double minValue = result[0];
        for (std::size_t i = 1; i < 4; i++) {
            minValue = result[i] > minValue ? result[i] : minValue;
        }
        /* find min value in the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            minValue = px[i] > minValue ? px[i] : minValue;
        }
        return minValue;
    }

    inline static float min(const float* __restrict x, std::size_t N)
    {
        const float *px = x;
        /* align */
        std::size_t r = N%8;
        /* find min value per 8 element */
        __m256 minVec = _mm256_loadu_ps(px);
        for (std::size_t i = 8; i < N - r; i+=8) {
            __m256 vecx = _mm256_loadu_ps(px + i);
            minVec = _mm256_min_ps(vecx, minVec);
        }
        /* find min value in result */
        float result[8];
        _mm256_storeu_ps(result, minVec);
        float minValue = result[0];
        for (std::size_t i = 1; i < 8; i++) {
            minValue = result[i] > minValue ? result[i] : minValue;
        }
        /* find min value in the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            minValue = px[i] > minValue ? px[i] : minValue;
        }
        return minValue;
    }

    inline static double sum(const double* __restrict x, std::size_t N)
    {
        const double *px = x;
        /* init */
        std::size_t r = N%4;
        double result[4] = {0};
        __m256d vecs = _mm256_loadu_pd(result);
        for (std::size_t i = 0; i < N - r; i+=4) {
            __m256d vecx = _mm256_loadu_pd(px + i);
            vecs = _mm256_add_pd(vecs, vecx);
        }
        /* sum up result */
        _mm256_storeu_pd(result, vecs);
        double s = 0;
        for (std::size_t i = 0; i < 4; i++) {
            s += result[i];
        }
        /* sum up the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            s += px[i];
        }
        return s;
    }

    inline static float sum(const float* __restrict x, std::size_t N)
    {
        const float *px = x;
        /* init */
        std::size_t r = N%8;
        __m256 vecs = _mm256_setzero_ps();
        for (std::size_t i = 0; i < N - r; i+=8) {
            __m256 vecx = _mm256_loadu_ps(px + i);
            vecs = _mm256_add_ps(vecs, vecx);
        }
        /* sum up result */
        float s = reduce(vecs);
        /* sum up the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            s += px[i];
        }
        return s;
    }
    inline static double product(const double* __restrict x, std::size_t N)
    {
        const double *px = x;
        /* init */
        std::size_t r = N%4;
        double result[4] = {0};
        __m256d vecx;
        __m256d vecs = _mm256_setr_pd(1, 1, 1, 1);
        for (std::size_t i = 0; i < N - r; i+=4) {
            vecx = _mm256_loadu_pd(px + i);
            vecs = _mm256_mul_pd(vecs, vecx);
        }
        float s = 1;
        _mm256_storeu_pd(result, vecs);
        for (std::size_t i = 0; i < 4; i++) {
            s *= result[i];
        }
        /* the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            s *= px[i];
        }
        return s;
    }

    inline static float product(const float* __restrict x, std::size_t N)
    {
        const float *px = x;
        /* init */
        std::size_t r = N%8;
        float result[8] = {0};
        __m256 vecs = _mm256_setr_ps(1, 1, 1, 1, 1, 1, 1, 1);
        for (std::size_t i = 0; i < N - r; i+=8) {
            __m256 vecx = _mm256_loadu_ps(px + i);
            vecs = _mm256_mul_ps(vecs, vecx);
        }
        float s = 1;
        _mm256_storeu_ps(result, vecs);
        for (std::size_t i = 0; i < 8; i++) {
            s *= result[i];
        }
        /* the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            s *= px[i];
        }
        return s;
    }

    inline static void sqrt(double* __restrict y, const double* __restrict x, std::size_t N)
    {
        const double *px = x;
        double *py = y;
        std::size_t r = N%4;
        __m256d vecx;
        __m256d vecy;
        for (std::size_t i = 0; i < N - r; i+=4) {
            vecx = _mm256_loadu_pd(px + i);
            vecy = _mm256_sqrt_pd(vecx);
            _mm256_storeu_pd(py + i, vecy);
        }
        /* sqrt the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            py[i] = std::sqrt(px[i]);
        }
        return;
    }

    inline static void sqrt(float* __restrict y, const float* __restrict x, std::size_t N)
    {
        const float *px = x;
        float *py = y;
        std::size_t r = N%8;
        __m256 vecx;
        __m256 vecy;
        for (std::size_t i = 0; i < N - r; i+=8) {
            vecx = _mm256_loadu_ps(px + i);
            vecy = _mm256_sqrt_ps(vecx);
            _mm256_storeu_ps(py + i, vecy);
        }
        /* sqrt the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            py[i] = std::sqrt(px[i]);
        }
        return;
    }

    inline static __m256 exp256_ps(__m256& x)
    {
        /*
            origin:
                http://software-lisc.fbk.eu/avx_mathfun/avx_mathfun.h
        */
        __m256   exp_hi        = _mm256_set1_ps(88.3762626647949f);
        __m256   exp_lo        = _mm256_set1_ps(-88.3762626647949f);

        __m256   cephes_LOG2EF = _mm256_set1_ps(1.44269504088896341);
        __m256   cephes_exp_C1 = _mm256_set1_ps(0.693359375);
        __m256   cephes_exp_C2 = _mm256_set1_ps(-2.12194440e-4);

        __m256   cephes_exp_p0 = _mm256_set1_ps(1.9875691500E-4);
        __m256   cephes_exp_p1 = _mm256_set1_ps(1.3981999507E-3);
        __m256   cephes_exp_p2 = _mm256_set1_ps(8.3334519073E-3);
        __m256   cephes_exp_p3 = _mm256_set1_ps(4.1665795894E-2);
        __m256   cephes_exp_p4 = _mm256_set1_ps(1.6666665459E-1);
        __m256   cephes_exp_p5 = _mm256_set1_ps(5.0000001201E-1);
        __m256   tmp           = _mm256_setzero_ps(), fx;
        __m256i  imm0;
        __m256   one           = _mm256_set1_ps(1.0f);

        x     = _mm256_min_ps(x, exp_hi);
        x     = _mm256_max_ps(x, exp_lo);

        /* express exp(x) as exp(g + n*log(2)) */
        fx    = _mm256_mul_ps(x, cephes_LOG2EF);
        fx    = _mm256_add_ps(fx, _mm256_set1_ps(0.5f));
        tmp   = _mm256_floor_ps(fx);
        __m256  mask  = _mm256_cmp_ps(tmp, fx, _CMP_GT_OS);
        mask  = _mm256_and_ps(mask, one);
        fx    = _mm256_sub_ps(tmp, mask);
        tmp   = _mm256_mul_ps(fx, cephes_exp_C1);
        __m256  z     = _mm256_mul_ps(fx, cephes_exp_C2);
        x     = _mm256_sub_ps(x, tmp);
        x     = _mm256_sub_ps(x, z);
        z     = _mm256_mul_ps(x,x);

        __m256  y = cephes_exp_p0;
        y     = _mm256_mul_ps(y, x);
        y     = _mm256_add_ps(y, cephes_exp_p1);
        y     = _mm256_mul_ps(y, x);
        y     = _mm256_add_ps(y, cephes_exp_p2);
        y     = _mm256_mul_ps(y, x);
        y     = _mm256_add_ps(y, cephes_exp_p3);
        y     = _mm256_mul_ps(y, x);
        y     = _mm256_add_ps(y, cephes_exp_p4);
        y     = _mm256_mul_ps(y, x);
        y     = _mm256_add_ps(y, cephes_exp_p5);
        y     = _mm256_mul_ps(y, z);
        y     = _mm256_add_ps(y, x);
        y     = _mm256_add_ps(y, one);

        /* build 2^n */
        imm0  = _mm256_cvttps_epi32(fx);
        imm0  = _mm256_add_epi32(imm0, _mm256_set1_epi32(0x7f));
        imm0  = _mm256_slli_epi32(imm0, 23);
        __m256  pow2n = _mm256_castsi256_ps(imm0);
        y     = _mm256_mul_ps(y, pow2n);
        return y;
    }

    inline static void exp(float* __restrict y, const float* __restrict x, std::size_t N)
    {
        const float *px = x;
        float *py = y;
        std::size_t r = N%8;
        __m256 vecx;
        __m256 vecy;
        for (std::size_t i = 0; i < N - r; i+=8) {
            vecx = _mm256_loadu_ps(px + i);
            vecy = exp256_ps(vecx);
            _mm256_storeu_ps(py + i, vecy);
        }
        /* sqrt the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            py[i] = std::exp(px[i]);
        }
        return;
    }

    inline static double dot(const double* __restrict x1, const double* __restrict x2, std::size_t N)
    {
        const double *px1 = x1;
        const double *px2 = x2;
        std::size_t r = N%4;
        __m256d vecx1;
        __m256d vecx2;
        __m256d vecy = _mm256_setzero_pd();
        for (std::size_t i = 0; i < N - r; i+=4) {
            vecx1 = _mm256_loadu_pd(px1 + i);
            vecx2 = _mm256_loadu_pd(px2 + i);
            vecy = _mm256_fmadd_pd(vecx1, vecx2, vecy);
        }
        /* sum up result */
        double s = reduce(vecy);
        /* dot the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            s += px1[i]*px2[i];
        }
        return s;
    }

    inline static float dot(const float* __restrict x1,
                           const float* __restrict x2,
                           std::size_t N)
    {
        const float *px1 = x1;
        const float *px2 = x2;
        std::size_t r = N%8;
        __m256 vecx1;
        __m256 vecx2;
        __m256 vecy = _mm256_setzero_ps();
        for (std::size_t i = 0; i < N - r; i+=8) {
            vecx1 = _mm256_loadu_ps(px1 + i);
            vecx2 = _mm256_loadu_ps(px2 + i);
            vecy = _mm256_fmadd_ps(vecx1, vecx2, vecy);
        }
        /* sum up result */
        float s = reduce(vecy);
        /* dot the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            s += px1[i]*px2[i];
        }
        return s;
    }

    inline static double norm2s(const double* __restrict x1, const double* __restrict x2, std::size_t N)
    {
        const double *px1 = x1;
        const double *px2 = x2;
        std::size_t r = N%4;
        __m256d vecx1;
        __m256d vecx2;
        __m256d vecx;
        __m256d vecy = _mm256_setzero_pd();
        for (std::size_t i = 0; i < N - r; i+=4) {
            vecx1 = _mm256_loadu_pd(px1 + i);
            vecx2 = _mm256_loadu_pd(px2 + i);
            vecx = _mm256_sub_pd(vecx1, vecx2);
            vecy = _mm256_fmadd_pd(vecx, vecx, vecy);
        }
        /* sum up result */
        double s = reduce(vecy);
        /* the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            s += (px1[i] - px2[i])*(px1[i] - px2[i]);
        }
        return s;
    }

    inline static float norm2s(const float* __restrict x1, const float* __restrict x2, std::size_t N)
    {
        const float *px1 = x1;
        const float *px2 = x2;
        std::size_t r = N%8;
        __m256 vecx1;
        __m256 vecx2;
        __m256 vecx;
        __m256 vecy = _mm256_setzero_ps();
        for (std::size_t i = 0; i < N - r; i+=8) {
            vecx1 = _mm256_loadu_ps(px1 + i);
            vecx2 = _mm256_loadu_ps(px2 + i);
            vecx = _mm256_sub_ps(vecx1, vecx2);
            vecy = _mm256_fmadd_ps(vecx, vecx, vecy);
        }
        /* sum up result */
        float s = reduce(vecy);
        /* the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            s += (px1[i] - px2[i])*(px1[i] - px2[i]);
        }
        return s;
    }

    inline static double variance(const double* __restrict x,  double  u, std::size_t N)
    {
        const double *px = x;
        std::size_t r = N%4;
        __m256d vecx1;
        __m256d vecu = _mm256_set1_pd(u);
        __m256d vecx;
        __m256d vecy;
        for (std::size_t i = 0; i < N - r; i+=4) {
            vecx1 = _mm256_loadu_pd(px + i);
            vecx = _mm256_sub_pd(vecx1, vecu);
            vecy = _mm256_fmadd_pd(vecx, vecx, vecy);
        }
        /* sum up result */
        double s = reduce(vecy);
        /* the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            s += (px[i] - u)*(px[i] - u);
        }
        return s/double(N);
    }

    inline static float variance(const float* __restrict x,  float  u, std::size_t N)
    {
        const float *px = x;
        std::size_t r = N%8;
        __m256 vecx1;
        __m256 vecu = _mm256_set1_ps(u);
        __m256 vecx;
        __m256 vecy;
        for (std::size_t i = 0; i < N - r; i+=8) {
            vecx1 = _mm256_loadu_ps(px + i);
            vecx = _mm256_sub_ps(vecx1, vecu);
            vecy = _mm256_fmadd_ps(vecx, vecx, vecy);
        }
        /* sum up result */
        float s = reduce(vecy);
        /* the rest elements */
        for (std::size_t i = N - r; i < N; i++) {
            s += (px[i] - u)*(px[i] - u);
        }
        return s/float(N);
    }

    inline static void matMul(double* __restrict z, std::size_t zRow, std::size_t zCol,
                              const double* __restrict x, std::size_t xRow, std::size_t xCol,
                              const double* __restrict y, std::size_t yRow, std::size_t yCol)
    {
        const double *x_ = x;
        const double *y_ = y;
        double *z_ = z;
        std::size_t r = zCol%4;
        for (std::size_t i = 0; i < zRow; i++) {
            for (std::size_t k = 0; k < xCol; k++) {
                double xik = x_[i*xCol + k];
                __m256d vecx = _mm256_set1_pd(xik);
                for (std::size_t j = 0; j < zCol - r; j+=4) {
                    __m256d vecy = _mm256_loadu_pd(y_ + k*yCol + j);
                    __m256d vecz = _mm256_loadu_pd(z_ + i*xCol + j);
                    /* _mm256_fmadd_ps(a, b, c): a*b + c */
                    vecz = _mm256_fmadd_pd(vecx, vecy, vecz);
                    /* store result */
                    _mm256_storeu_pd(z_ + i*xCol + j, vecz);
                }
                for (std::size_t j = zCol - r; j < zCol; j++) {
                    z_[i*zCol + j] += xik * y_[k*yCol + j];
                }
            }
        }
        return;
    }

    inline static void matMul16(double* __restrict z, std::size_t zRow, std::size_t zCol,
                                const double* __restrict x, std::size_t xRow, std::size_t xCol,
                                const double* __restrict y, std::size_t yRow, std::size_t yCol)
    {
        const double *x_ = x;
        const double *y_ = y;
        double *z_ = z;
        std::size_t r1 = xCol%4;
        std::size_t r2 = zCol%4;
        double xik[4];
        __m256d vecx[4];
        __m256d vecy[4];
        __m256d vecz;
        for (std::size_t i = 0; i < zRow; i++) {
            for (std::size_t k = 0; k < xCol - r1; k+=4) {
                xik[0] = x_[i*xCol + k];
                xik[1] = x_[i*xCol + k + 1];
                xik[2] = x_[i*xCol + k + 2];
                xik[3] = x_[i*xCol + k + 3];
                vecx[0] = _mm256_set1_pd(xik[0]);
                vecx[1] = _mm256_set1_pd(xik[1]);
                vecx[2] = _mm256_set1_pd(xik[2]);
                vecx[3] = _mm256_set1_pd(xik[3]);
                for (std::size_t j = 0; j < zCol - r2; j+=4) {
                    /* put 8 float into __m256 */
                    vecy[0] = _mm256_loadu_pd(y_ + k*yCol + j);
                    vecy[1] = _mm256_loadu_pd(y_ + (k + 1)*yCol + j);
                    vecy[2] = _mm256_loadu_pd(y_ + (k + 2)*yCol + j);
                    vecy[3] = _mm256_loadu_pd(y_ + (k + 3)*yCol + j);
                    vecz  = _mm256_loadu_pd(z_ + i*xCol + j);
                    /* _mm256_fmadd_ps(a, b, c): a*b + c */
                    vecz = _mm256_fmadd_pd(vecx[0], vecy[0], vecz);
                    vecz = _mm256_fmadd_pd(vecx[1], vecy[1], vecz);
                    vecz = _mm256_fmadd_pd(vecx[2], vecy[2], vecz);
                    vecz = _mm256_fmadd_pd(vecx[3], vecy[3], vecz);
                    /* store result */
                    _mm256_storeu_pd(z_ + i*xCol + j, vecz);
                }
                for (std::size_t j = zCol - r2; j < zCol; j++) {
                    z_[i*zCol + j] += xik[0] * y_[k*yCol + j];
                    z_[i*zCol + j] += xik[1] * y_[(k + 1)*yCol + j];
                    z_[i*zCol + j] += xik[2] * y_[(k + 2)*yCol + j];
                    z_[i*zCol + j] += xik[3] * y_[(k + 3)*yCol + j];
                }
            }
        }
        return;
    }

    inline static void matMul(float* __restrict z, std::size_t zRow, std::size_t zCol,
                              const float* __restrict x, std::size_t xRow, std::size_t xCol,
                              const float* __restrict y, std::size_t yRow, std::size_t yCol)
    {
        /*
            origin:
                https://blog.csdn.net/StandCrow/article/details/120206063
        */
        const float *x_ = x;
        const float *y_ = y;
        float *z_ = z;
        std::size_t r = zCol%8;
        __m256 vecx;
        __m256 vecy;
        __m256 vecz;
        for (std::size_t i = 0; i < zRow; i++) {
            for (std::size_t k = 0; k < xCol; k++) {
                float xik = x_[i*xCol + k];
                vecx = _mm256_set1_ps(xik);
                for (std::size_t j = 0; j < zCol - r; j+=8) {
                    /* put 8 float into __m256 */
                    vecy = _mm256_loadu_ps(y_ + k*yCol + j);
                    vecz = _mm256_loadu_ps(z_ + i*xCol + j);
                    /* _mm256_fmadd_ps(a, b, c): a*b + c */
                    vecz = _mm256_fmadd_ps(vecx, vecy, vecz);
                    /* store result */
                    _mm256_storeu_ps(z_ + i*xCol + j, vecz);
                }
                /* the rest column */
                for (std::size_t j = zCol - r; j < zCol; j++) {
                    z_[i*zCol + j] += xik * y_[k*yCol + j];
                }
            }
        }
        return;
    }

    inline static void matMul64(float* __restrict z, std::size_t zRow, std::size_t zCol,
                                const float* __restrict x, std::size_t xRow, std::size_t xCol,
                                const float* __restrict y, std::size_t yRow, std::size_t yCol)
    {
        const float *x_ = x;
        const float *y_ = y;
        float *z_ = z;
        std::size_t r1 = xCol%8;
        std::size_t r2 = zCol%8;
        float xik[8];
        __m256 vecx[8];
        __m256 vecy[8];
        __m256 vecz;
        for (std::size_t i = 0; i < zRow; i++) {
            for (std::size_t k = 0; k < xCol - r1; k+=8) {
                xik[0] = x_[i*xCol + k];
                xik[1] = x_[i*xCol + k + 1];
                xik[2] = x_[i*xCol + k + 2];
                xik[3] = x_[i*xCol + k + 3];
                xik[4] = x_[i*xCol + k + 4];
                xik[5] = x_[i*xCol + k + 5];
                xik[6] = x_[i*xCol + k + 6];
                xik[7] = x_[i*xCol + k + 7];
                vecx[0] = _mm256_set1_ps(xik[0]);
                vecx[1] = _mm256_set1_ps(xik[1]);
                vecx[2] = _mm256_set1_ps(xik[2]);
                vecx[3] = _mm256_set1_ps(xik[3]);
                vecx[4] = _mm256_set1_ps(xik[4]);
                vecx[5] = _mm256_set1_ps(xik[5]);
                vecx[6] = _mm256_set1_ps(xik[6]);
                vecx[7] = _mm256_set1_ps(xik[7]);
                for (std::size_t j = 0; j < zCol - r2; j+=8) {
                    /* put 8 float into __m256 */
                    vecy[0] = _mm256_loadu_ps(y_ + k*yCol + j);
                    vecy[1] = _mm256_loadu_ps(y_ + (k + 1)*yCol + j);
                    vecy[2] = _mm256_loadu_ps(y_ + (k + 2)*yCol + j);
                    vecy[3] = _mm256_loadu_ps(y_ + (k + 3)*yCol + j);
                    vecy[4] = _mm256_loadu_ps(y_ + (k + 4)*yCol + j);
                    vecy[5] = _mm256_loadu_ps(y_ + (k + 5)*yCol + j);
                    vecy[6] = _mm256_loadu_ps(y_ + (k + 6)*yCol + j);
                    vecy[7] = _mm256_loadu_ps(y_ + (k + 7)*yCol + j);
                    vecz  = _mm256_loadu_ps(z_ + i*xCol + j);
                    /* _mm256_fmadd_ps(a, b, c): a*b + c */
                    vecz = _mm256_fmadd_ps(vecx[0], vecy[0], vecz);
                    vecz = _mm256_fmadd_ps(vecx[1], vecy[1], vecz);
                    vecz = _mm256_fmadd_ps(vecx[2], vecy[2], vecz);
                    vecz = _mm256_fmadd_ps(vecx[3], vecy[3], vecz);
                    vecz = _mm256_fmadd_ps(vecx[4], vecy[4], vecz);
                    vecz = _mm256_fmadd_ps(vecx[5], vecy[5], vecz);
                    vecz = _mm256_fmadd_ps(vecx[6], vecy[6], vecz);
                    vecz = _mm256_fmadd_ps(vecx[7], vecy[7], vecz);
                    /* store result */
                    _mm256_storeu_ps(z_ + i*xCol + j, vecz);
                }

                for (std::size_t j = zCol - r2; j < zCol; j++) {
                    z_[i*zCol + j] += xik[0] * y_[k*yCol + j];
                    z_[i*zCol + j] += xik[1] * y_[(k + 1)*yCol + j];
                    z_[i*zCol + j] += xik[2] * y_[(k + 2)*yCol + j];
                    z_[i*zCol + j] += xik[3] * y_[(k + 3)*yCol + j];
                    z_[i*zCol + j] += xik[4] * y_[(k + 4)*yCol + j];
                    z_[i*zCol + j] += xik[5] * y_[(k + 5)*yCol + j];
                    z_[i*zCol + j] += xik[6] * y_[(k + 6)*yCol + j];
                    z_[i*zCol + j] += xik[7] * y_[(k + 7)*yCol + j];
                }
            }
        }
        return;
    }

    inline static void conv2d(float *y, std::size_t yC, std::size_t yH, std::size_t yW,
                              const float *kernels, std::size_t kN, std::size_t kC, std::size_t kH, std::size_t kW,
                              const float *x, std::size_t xC, std::size_t xH, std::size_t xW,
                              std::size_t stride=1, std::size_t padding=0)
    {
        const float *x_ = x;
        const float *kernels_ = kernels;
        float *y_ = y;
        __m256 vecx;
        __m256 vecy;
        __m256 vecz;
        std::size_t sK = kH*kW;
        std::size_t sY = yH*yW;
        std::size_t sX = xH*xW;
        std::size_t r = sK%8;
        for (std::size_t n = 0; n < yC; n++) {
            for (std::size_t i = 0; i < yH; i++) {
                for (std::size_t j = 0; j < yW; j++) {

                    std::size_t nij = n*sY + i*yW + j;
                    /* kernels */
                    for (std::size_t c = 0; c < kC; c++) {


                        for (std::size_t si = 0; si < sK; si++) {
                            std::size_t h = si/kW;
                            std::size_t k = si%kW;

                            /* map to input  */
                            std::size_t row = h + i*stride - padding;
                            std::size_t col = k + j*stride - padding;
                            if (row < 0 || row >= xH || col < 0 || col >= xW) {
                                continue;
                            }

                            std::size_t nchk = n*(kC*sK) + c*sK + h*kW + k;
                            std::size_t chw = c*sX + row*xW + col;
                            /* sum up all convolution result */

                            y_[nij] += kernels_[nchk]*x_[chw];

                        }

                    }
                }
            }
        }
        return;
    }

    inline static void transpose(double* __restrict y, std::size_t yRow, std::size_t yCol,
                                 const double* __restrict x, std::size_t xRow, std::size_t xCol)
    {

        /*
            origin:
                https://blog.csdn.net/artorias123/article/details/86524899

                suqure matrix required

            a0,a1,a2,a3             a0,b0,c0,d0
            b0,b1,b2,b3     ->      a1,b1,c1,d1
            c0,c1,c2,c3             a2,b2,c2,d2
            d0,d1,d2,d3             a3,b3,c3,d3

            __m256d = 4 x double
            +--+--+--+--+
            |a1|a2|a3|a4|   low: a1,a3    high: a2,a4
            +--+--+--+--+

            1. _mm256_unpacklo_pd:
                    pick the lows and sort
                    [(a0,a1,a2,a3), (b0,b1,b2,b3)] -> (a0,b0,a2,b2)

            2. _mm256_unpackhi_pd:
                    pick the highs and sort
                    [(a0,a1,a2,a3), (b0,b1,b2,b3)] -> (a1,b1,a3,b3)

            3. _mm256_permute4x64_pd:
                    permute elements by specific order
                    [(a0,a1,a2,a3), (3,2,1,0)b] -> (a3,a2,a1,a0),
                    (3,2,1,0)b -> (11,10,01,00) -> (00,01,10,11) -> 00011011 ->0x1b

            4. _mm256_blend_pd:
                    pick elements from a and b by paramter
                    [(a0,a1,a2,a3), (b0,b1,b2,b3), 0101] -> (a0,b1,a2,b3)
                    0101: 0 -> pick a, 1 -> pick b
            5. transpose4x4:
                    5.1 read all rows:
                                           a0,a1,a2,a3
                                     A =   b0,b1,b2,b3
                                           c0,c1,c2,c3
                                           d0,d1,d2,d3

                    5.2 permute each rows:
                        (2,3,0,1) -> (1,0,3,2) -> 01001110 -> 0x4e
                                            a2,a3,a0,a1
                        (2,3,0,1)A ->  P =  b2,b3,b0,b1
                                            c2,c3,c0,c1
                                            d2,d3,d0,d1

                    5.3 blend rows: 1100 -> 0x0c

                        (A0, P2)            a0,a1,c0,c1
                        (A1, P3)  ->  B =   b0,b1,d0,d1
                        (P0, A2)            a2,a3,c2,c3
                        (P1, A3)            b2,b3,d2,d3

                    5.4 pick low and high between rows:

                        pick low:  (B0,B1)      a0,b0,c0,d0
                        pick high: (B0,B1)  ->  a1,b1,c1,d1
                        pick low:  (B2,B3)      a2,b2,c2,d2
                        pick high: (B2,B3)      a3,b3,c3,d3

                    5.5 store

              6. transpose from A4x4 to Anxn


        */
        const double *px0 = x;
        const double *px1 = x + xCol;
        const double *px2 = x + 2*xCol;
        const double *px3 = x + 3*xCol;
        double *py0 = y;
        double *py1 = y + xCol;
        double *py2 = y + 2*xCol;
        double *py3 = y + 3*xCol;
        double *t = y + 4;
        __m256d A[4];
        __m256d P[4];
        __m256d B[4];
        __m256d C[4];
        for (std::size_t i = 0; i < xRow/4; i++) {
            for (std::size_t j = 0; j < xCol/4; j++) {
                /* load row elements */
                A[0] = _mm256_loadu_pd(px0);
                A[1] = _mm256_loadu_pd(px1);
                A[2] = _mm256_loadu_pd(px2);
                A[3] = _mm256_loadu_pd(px3);
                /* permute each rows: (0, 1, 2, 3) -> (2, 3, 0, 1) */
                P[0] = _mm256_permute4x64_pd(A[0], 0x4E);
                P[1] = _mm256_permute4x64_pd(A[1], 0x4E);
                P[2] = _mm256_permute4x64_pd(A[2], 0x4E);
                P[3] = _mm256_permute4x64_pd(A[3], 0x4E);
                /* blend */
                B[0] = _mm256_blend_pd(A[0], P[2], 0x0C);
                B[1] = _mm256_blend_pd(A[1], P[3], 0x0C);
                B[2] = _mm256_blend_pd(P[0], A[2], 0x0C);
                B[3] = _mm256_blend_pd(P[1], A[3], 0x0C);
                /* pick low and high */
                C[0] = _mm256_unpacklo_pd(B[0], B[1]);
                C[1] = _mm256_unpackhi_pd(B[0], B[1]);
                C[2] = _mm256_unpacklo_pd(B[2], B[3]);
                C[3] = _mm256_unpackhi_pd(B[2], B[3]);
                /* store */
                _mm256_storeu_pd(py0, C[0]);
                _mm256_storeu_pd(py1, C[1]);
                _mm256_storeu_pd(py2, C[2]);
                _mm256_storeu_pd(py3, C[3]);

                px0 += 4; py0 += 4*xCol;
                px1 += 4; py1 += 4*xCol;
                px2 += 4; py2 += 4*xCol;
                px3 += 4; py3 += 4*xCol;

            }
            px0 += 3*xRow; py0 = t;
            px1 += 3*xRow; py1 = t + xRow;
            px2 += 3*xRow; py2 = t + 2*xRow;
            px3 += 3*xRow; py3 = t + 3*xRow;
            t += 4;
        }
        return;
    }

    inline static void transpose(float* __restrict y, std::size_t yRow, std::size_t yCol,
                                 const float* __restrict x, std::size_t xRow, std::size_t xCol)
    {

        const float *x_ = x;
        float *y_ = y;
        std::size_t r1 = xRow%8;
        std::size_t r2 = xCol%8;
        for (std::size_t i = 0; i < xRow - r1; i+=8) {
            for (std::size_t j = 0; j < xCol - r2; j+=8) {
                __m256 r[8];
                __m256 t[8];
                __m128 x1f[8];
                __m128 x2f[8];
                x1f[0] = _mm_load_ps(x_ + 0*8 + 0);
                x1f[1] = _mm_load_ps(x_ + 1*8 + 0);
                x1f[2] = _mm_load_ps(x_ + 2*8 + 0);
                x1f[3] = _mm_load_ps(x_ + 3*8 + 0);
                x1f[4] = _mm_load_ps(x_ + 0*8 + 4);
                x1f[5] = _mm_load_ps(x_ + 1*8 + 4);
                x1f[6] = _mm_load_ps(x_ + 2*8 + 4);
                x1f[7] = _mm_load_ps(x_ + 3*8 + 4);

                x2f[0] = _mm_load_ps(x_ + 4*8 + 0);
                x2f[1] = _mm_load_ps(x_ + 5*8 + 0);
                x2f[2] = _mm_load_ps(x_ + 6*8 + 0);
                x2f[3] = _mm_load_ps(x_ + 7*8 + 0);
                x2f[4] = _mm_load_ps(x_ + 4*8 + 4);
                x2f[5] = _mm_load_ps(x_ + 5*8 + 4);
                x2f[6] = _mm_load_ps(x_ + 6*8 + 4);
                x2f[7] = _mm_load_ps(x_ + 7*8 + 4);

                r[0] = _mm256_insertf128_ps(_mm256_castps128_ps256(x1f[0]), x2f[0], 1);
                r[1] = _mm256_insertf128_ps(_mm256_castps128_ps256(x1f[1]), x2f[1], 1);
                r[2] = _mm256_insertf128_ps(_mm256_castps128_ps256(x1f[2]), x2f[2], 1);
                r[3] = _mm256_insertf128_ps(_mm256_castps128_ps256(x1f[3]), x2f[3], 1);
                r[4] = _mm256_insertf128_ps(_mm256_castps128_ps256(x1f[4]), x2f[4], 1);
                r[5] = _mm256_insertf128_ps(_mm256_castps128_ps256(x1f[5]), x2f[5], 1);
                r[6] = _mm256_insertf128_ps(_mm256_castps128_ps256(x1f[6]), x2f[6], 1);
                r[7] = _mm256_insertf128_ps(_mm256_castps128_ps256(x1f[7]), x2f[7], 1);

                t[0] = _mm256_unpacklo_ps(r[0], r[1]);
                t[1] = _mm256_unpackhi_ps(r[0], r[1]);
                t[2] = _mm256_unpacklo_ps(r[2], r[3]);
                t[3] = _mm256_unpackhi_ps(r[2], r[3]);
                t[4] = _mm256_unpacklo_ps(r[4], r[5]);
                t[5] = _mm256_unpackhi_ps(r[4], r[5]);
                t[6] = _mm256_unpacklo_ps(r[6], r[7]);
                t[7] = _mm256_unpackhi_ps(r[6], r[7]);

                __m256 v;

                v = _mm256_shuffle_ps(t[0], t[2], 0x4E);
                r[0] = _mm256_blend_ps(t[0], v, 0xCC);
                r[1] = _mm256_blend_ps(t[2], v, 0x33);

                v = _mm256_shuffle_ps(t[1], t[3], 0x4E);
                r[2] = _mm256_blend_ps(t[1], v, 0xCC);
                r[3] = _mm256_blend_ps(t[3], v, 0x33);

                v = _mm256_shuffle_ps(t[4],t[6], 0x4E);
                r[4] = _mm256_blend_ps(t[4], v, 0xCC);
                r[5] = _mm256_blend_ps(t[6], v, 0x33);

                v = _mm256_shuffle_ps(t[5],t[7], 0x4E);
                r[6] = _mm256_blend_ps(t[5], v, 0xCC);
                r[7] = _mm256_blend_ps(t[7], v, 0x33);

                _mm256_store_ps(y_ + 0*8, r[0]);
                _mm256_store_ps(y_ + 1*8, r[1]);
                _mm256_store_ps(y_ + 2*8, r[2]);
                _mm256_store_ps(y_ + 3*8, r[3]);
                _mm256_store_ps(y_ + 4*8, r[4]);
                _mm256_store_ps(y_ + 5*8, r[5]);
                _mm256_store_ps(y_ + 6*8, r[6]);
                _mm256_store_ps(y_ + 7*8, r[7]);

            }
        }


        return;
    }

    struct MatMul {

        inline static void ikkj(float* __restrict z, std::size_t zRow, std::size_t zCol,
                                const float* __restrict x, std::size_t xRow, std::size_t xCol,
                                const float* __restrict y, std::size_t yRow, std::size_t yCol)
        {
            const float *x_ = x;
            const float *y_ = y;
            float *z_ = z;
            std::size_t r = zCol%8;
            __m256 vecx;
            __m256 vecy;
            __m256 vecz;
            for (std::size_t i = 0; i < zRow; i++) {
                for (std::size_t k = 0; k < xCol; k++) {
                    float xik = x_[i*xCol + k];
                    vecx = _mm256_set1_ps(xik);
                    for (std::size_t j = 0; j < zCol - r; j+=8) {
                        /* put 8 float into __m256 */
                        vecy = _mm256_loadu_ps(y_ + k*yCol + j);
                        vecz = _mm256_loadu_ps(z_ + i*xCol + j);
                        /* _mm256_fmadd_ps(a, b, c): a*b + c */
                        vecz = _mm256_fmadd_ps(vecx, vecy, vecz);
                        /* store result */
                        _mm256_storeu_ps(z_ + i*xCol + j, vecz);
                    }
                    for (std::size_t j = zCol - r; j < zCol; j++) {
                        z_[i*zCol + j] += xik * y_[k*yCol + j];
                    }
                }
            }
            return;
        }

        static void kikj(float* __restrict z, std::size_t zRow, std::size_t zCol,
                         const float* __restrict x, std::size_t xRow, std::size_t xCol,
                         const float* __restrict y, std::size_t yRow, std::size_t yCol)
        {
            /* z = x^T * y */
            const float *x_ = x;
            const float *y_ = y;
            float *z_ = z;
            std::size_t r = zCol%8;
            __m256 vecx;
            __m256 vecy;
            __m256 vecz;
            for (std::size_t i = 0; i < zRow; i++) {
                for (std::size_t k = 0; k < xRow; k++) {
                    float xki = x_[k*xCol + i];
                    vecx = _mm256_set1_ps(xki);
                    for (std::size_t j = 0; j < zCol - r; j+=8) {
                        /* put 8 float into __m256 */
                        vecy = _mm256_loadu_ps(y_ + k*yCol + j);
                        vecz = _mm256_loadu_ps(z_ + i*xCol + j);
                        /* _mm256_fmadd_ps(a, b, c): a*b + c */
                        vecz = _mm256_fmadd_ps(vecx, vecy, vecz);
                        /* store result */
                        _mm256_storeu_ps(z_ + i*xCol + j, vecz);
                    }
                    for (std::size_t j = zCol - r; j < zCol; j++) {
                        z_[i*zCol + j] += xki * y_[k*yCol + j];
                    }
                }
            }
            return;
        }
        static void ikjk(float* __restrict z, std::size_t zRow, std::size_t zCol,
                         const float* __restrict x, std::size_t xRow, std::size_t xCol,
                         const float* __restrict y, std::size_t yRow, std::size_t yCol)
        {
            /* z = x * y^T */
            const float *x_ = x;
            const float *y_ = y;
            float *z_ = z;
            std::size_t r = zCol%8;
            __m256 vecx;
            __m256 vecy;
            __m256 vecz;
            for (std::size_t i = 0; i < zRow; i++) {
                for (std::size_t k = 0; k < xCol; k++) {
                    float xik = x_[i*xCol + k];
                    vecx = _mm256_set1_ps(xik);
                    for (std::size_t j = 0; j < zCol - r; j+=8) {
                        /* put 8 float into __m256 */
                        vecy = _mm256_loadu_ps(y_ + k*yCol + j);
                        vecz = _mm256_loadu_ps(z_ + i*xCol + j);
                        /* _mm256_fmadd_ps(a, b, c): a*b + c */
                        vecz = _mm256_fmadd_ps(vecx, vecy, vecz);
                        /* store result */
                        _mm256_storeu_ps(z_ + i*xCol + j, vecz);
                    }
                    for (std::size_t j = zCol - r; j < zCol; j++) {
                        z_[i*zCol + j] += xik * y_[j*yCol + k];
                    }
                }
            }
            return;
        }
        static void kijk(float* __restrict z, std::size_t zRow, std::size_t zCol,
                         const float* __restrict x, std::size_t xRow, std::size_t xCol,
                         const float* __restrict y, std::size_t yRow, std::size_t yCol)
        {
            /* z = x^T * y^T */
            const float *x_ = x;
            const float *y_ = y;
            float *z_ = z;
            std::size_t r = zCol%8;
            __m256 vecx;
            __m256 vecy;
            __m256 vecz;
            for (std::size_t i = 0; i < zRow; i++) {
                for (std::size_t k = 0; k < xRow; k++) {
                    float xki = x_[k*xCol + i];
                    vecx = _mm256_set1_ps(xki);
                    for (std::size_t j = 0; j < zCol - r; j+=8) {
                        /* put 8 float into __m256 */
                        vecy = _mm256_loadu_ps(y_ + k*yCol + j);
                        vecz = _mm256_loadu_ps(z_ + i*xCol + j);
                        /* _mm256_fmadd_ps(a, b, c): a*b + c */
                        vecz = _mm256_fmadd_ps(vecx, vecy, vecz);
                        /* store result */
                        _mm256_storeu_ps(z_ + i*xCol + j, vecz);
                    }
                    for (std::size_t j = zCol - r; j < zCol; j++) {
                        z_[i*zCol + j] += xki * y_[j*yCol + k];
                    }
                }
            }
            return;
        }

    };

#endif // AVX2
};


}
#endif // AVX2FUNC_HPP
