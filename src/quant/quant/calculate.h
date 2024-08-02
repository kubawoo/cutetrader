#ifndef QUANT_CALCULATE_H
#define QUANT_CALCULATE_H

#include <numeric>
#include <cmath>

namespace quant {

class Calculate
{
private:
    Calculate();
public:
    template<typename Iterable>
    static double sum(const Iterable &x)
    {
        return std::accumulate(x.begin(), x.end(), 0.0);
    }

    template<typename Iterable>
    static double mean(const Iterable &x)
    {
        return sum(x) / x.size();
    }

    template<typename Iterable1, typename Iterable2>
    static double expectedValue(const Iterable1 &x, const Iterable2 &p)
    {
        return std::inner_product(x.begin(), x.end(), p.begin(), 0.0);
    }

    template<typename Iterable>
    static double variance(const Iterable &x)
    {
        double avg = mean(x);
        double total = 0.0;
        for(auto & i : x) {
            auto diff = avg - i;
            total += diff * diff;
        }
        return total / x.size();
    }

    template<typename Iterable1, typename Iterable2>
    static double variance(const Iterable1 &x, const Iterable2 &p)
    {
        double expected = expectedValue(x, p);
        double total = 0.0;

        auto itp = p.begin();
        auto itx = x.begin();

        for(; itx != x.end(); ++itx, ++itp) {
            auto vx = *itx;
            auto vp = *itp;
            total += vx*vx*vp;
        }
        return total - expected*expected;
    }

    template<typename Iterable>
    static double stdDev(const Iterable &x)
    {
        return sqrt(variance(x));
    }

    template<typename Iterable1, typename Iterable2>
    static double stdDev(const Iterable1 &x, const Iterable2 &p)
    {
        return sqrt(variance(x, p));
    }

    template<typename Iterable>
    static double skew(const Iterable &x)
    {
        double avg = mean(x);
        double total = 0.0;
        for(auto & i : x) {
            auto diff = avg - i;
            total += diff * diff * diff;
        }
        return total / x.size() / pow(stdDev(x), 3);
    }

    template<typename Iterable1, typename Iterable2>
    static double covariance(const Iterable1 &x, const Iterable2 &y)
    {
        double meanX = mean(x);
        double meanY = mean(y);
        double total = 0.0;

        auto itx = x.begin();
        auto ity = y.begin();

        for(; itx != x.end(); ++itx, ++ity) {
            auto vx = *itx;
            auto vy = *ity;
            total += (vx - meanX)*(vy - meanY);
        }
        return total / x.size();
    }

    template<typename Iterable1, typename Iterable2>
    static double correlation(const Iterable1 &x, const Iterable2 &y)
    {
        return covariance(x, y) / (stdDev(x) * stdDev(y));
    }

    template<typename Iterable1, typename Iterable2>
    static double beta(const Iterable1 &x, const Iterable2 &y)
    {
        return covariance(x, y) / variance(y);
    }

    template<typename Iterable>
    static double polynomial(const Iterable &a, const double x)
    {
        double result = a[0];
        for (int i=1; i<a.size(); i++) {
            result = result*x + a[i];
        }

        return result;
    }

};

}

#endif
