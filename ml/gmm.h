#ifndef GMM_H
#define GMM_H
#include <vector>
#include "../basic/tensor.hpp"
#include "../basic/linalg.h"

class GMM
{
public:
    int topicDim;
    int featureDim;
    Tensor alpha;
    std::vector<Tensor> u;
    std::vector<Tensor> sigma;
public:
    GMM(){}
    explicit GMM(int topicDim_, int featureDim_)
        :topicDim(topicDim_),featureDim(featureDim_)
    {
        alpha = Tensor(topicDim, 1);
        u = std::vector<Tensor>(topicDim, Tensor(featureDim, 1));
        sigma = std::vector<Tensor>(topicDim, Tensor(featureDim, 1));
    }

    static float gaussian(const Tensor &x, const Tensor &u, const Tensor &sigma)
    {
        /*
            x:(featureDim, 1)
            u:(featureDim, 1)
            sigma:(featureDim, featureDim)
            delta = x - u
        */
        int n = x.shape[0];
        Tensor delta(x.shape);
        LinAlg::sub(delta, x, u);
        /* r = (x - u)^T*isigma*(x - u) */
        Tensor r(x.shape[1], x.shape[1]);
        LinAlg::xTAx(r, delta, LinAlg::diagInv(sigma));
        float coeff = std::pow(2*LinAlg::pi, float(n)/2)*std::sqrt(LinAlg::det(sigma));
        return 1.0/(coeff + 1e-6)*std::exp(-0.5*r[0]);
    }

    void estimate(const std::vector<Tensor> &x, Tensor &nk, Tensor &gamma)
    {
        /*
           likelyhood
           x:(n, featureDim)
           u:(topicDim, featureDim)
           sigma:(topicDim, featureDim)
        */
        int n = x.size();
        Tensor p(n, topicDim);
        Tensor sp(n, 1);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < topicDim; j++) {
                float pij = alpha[j]*gaussian(x[i], u[j], LinAlg::diag(sigma[j]));
                p(i, j) = pij;
                sp[i] += pij;
            }
        }
        /* gamma:(n, topicDim) */
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < topicDim; j++) {
                gamma(i, j) = p(i, j)/(sp[i] + 1e-6);
            }
        }
        /* pk = gamma.sum(1) */
        for (int i = 0; i < topicDim; i++) {
            for (int j = 0; j < n; j++) {
                nk[i] += gamma(j, i);
            }
        }
        return;
    }

    void maximize(const std::vector<Tensor> &x, const Tensor &nk, const Tensor &gamma)
    {
        int n = x.size();
        /* pi */
        alpha = nk/float(n);
        /* sigma:(topicDim, featureDim)
           nk:(topicDim, 1)
           gamma:(n, topicDim)
           x:(n, featureDim)
           u:(topicDim, featureDim)
        */
        for (int i = 0; i < topicDim; i++) {
            sigma[i].zero();
        }

        for (int k = 0; k < topicDim; k++) {
            for (int j = 0; j < featureDim; j++) {
                for (int i = 0; i < n; i++) {
                    float d = x[i][j] - u[k][j];
                    sigma[k][j] += gamma(i, k)*d*d;
                }
            }
            sigma[k] /= nk[k];
        }
        /* u:(topicDim, featureDim)
           nk:(topicDim, 1)
           gamma:(n, topicDim)
           x:(n, featureDim)
        */
        for (int i = 0; i < topicDim; i++) {
            u[i].zero();
        }
        for (int k = 0; k < topicDim; k++) {
            for (int j = 0; j < featureDim; j++) {
                for (int i = 0; i < n; i++) {
                    u[k][j] += gamma(i, k)*x[i][j];
                }
            }
            u[k] /= nk[k];
        }
        return;
    }

    void cluster(const std::vector<Tensor> &x, std::size_t maxEpoch)
    {
        int n = x.size();
        /* init */
        alpha.fill(1.0/float(topicDim));
        std::uniform_int_distribution<int> uniform(0, n - 1);
        for (int i = 0; i < topicDim; i++) {
            int k = uniform(LinAlg::Random::engine);
            u[i] = x[k];
        }
        for (int k = 0; k < topicDim; k++) {
            for (int j = 0; j < featureDim; j++) {
                for (int i = 0; i < n; i++) {
                    float d = x[i][j] - u[k][j];
                    sigma[k][j] += d*d;
                }
            }
            sigma[k] /= featureDim;
        }
        /* iteration */
        Tensor nk(topicDim, 1);
        Tensor gamma(n, topicDim);
        for (std::size_t epoch = 0; epoch < maxEpoch; epoch++) {
            /* estimation */
            nk.zero();
            estimate(x, nk, gamma);
            /* maximization */
            maximize(x, nk, gamma);
            std::cout<<"epoch:"<<epoch<<std::endl;
        }
        return;
    }

    int operator()(const Tensor &xi)
    {
        float maxP = -1;
        int topic = 0;
        for (int i = 0; i < topicDim; i++) {
            float p = alpha[i]*gaussian(xi, u[i], LinAlg::diag(sigma[i]));
            if (p > maxP) {
                maxP = p;
                topic = i;
            }
        }
        return topic;
    }
};

#endif // GMM_H
