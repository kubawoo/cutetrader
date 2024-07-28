#ifndef QUANT_CALCULATE_H
#define QUANT_CALCULATE_H

#include <numeric>
#include <QDebug>

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
        double sum = 0.0;
        for(auto & i : x) {
            auto diff = avg - i;
            sum += diff * diff;
        }
        return sum / x.size();
    }

    template<typename Iterable1, typename Iterable2>
    static double variance(const Iterable1 &x, const Iterable2 &p)
    {
        double expected = expectedValue(x, p);

        qDebug() << expected;

        double sum = 0.0;

        auto itp = p.begin();
        auto itx = x.begin();

        for(; itx != x.end(); ++itx, ++itp) {
            auto vx = *itx;
            auto vp = *itp;
            sum += vx*vx*vp;
        }
        return sum - expected*expected;
    }
};

}

#endif // QUANT_CALCULATE_H
