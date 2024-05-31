
#include "Linq.h"

namespace Evade
{
    template<typename T>
    Linq<T>::Linq(const std::vector<T>& input) : data(input) {}

    template<typename T>
    Linq<T>::Linq(std::vector<T>&& input) : data(std::move(input)) {}

    template<typename T>
    void Linq<T>::AddRange(const Linq<T>& other)
    {
        data.insert(data.end(), other.data.begin(), other.data.end());
    }

    template<typename T>
    void Linq<T>::AddRange(const std::vector<T>& other)
    {
        data.insert(data.end(), other.begin(), other.end());
    }

    template<typename T>
    template<typename TReducer>
    auto Linq<T>::Aggregate(TReducer reducer) const
    {
        if (data.empty())
        {
            throw std::runtime_error("Sequence contains no elements.");
        }
        return std::accumulate(data.begin(), data.end(), data.front(), reducer);
    }

    template<typename T>
    template<typename TAccumulate, typename TReduce>
    TAccumulate Linq<T>::Aggregate(TAccumulate seed, TReduce reducer) const
    {
        return std::accumulate(data.begin(), data.end(), seed, reducer);
    }

    template<typename T>
    template<typename TAccumulate, typename TReduce, typename TSelector>
    TAccumulate Linq<T>::Aggregate(TAccumulate seed, TReduce reducer, TSelector selector) const
    {
        return std::accumulate(data.begin(), data.end(), seed, [&](const auto& acc, const auto& element)
        {
            return reducer(acc, selector(element));
        });
    }

    template<typename T>
    template<typename TPredicate>
    bool Linq<T>::All(TPredicate predicate) const
    {
        return std::all_of(data.begin(), data.end(), predicate);
    }

    template<typename T>
    bool Linq<T>::Any() const
    {
        return !data.empty();
    }

    template<typename T>
    template<typename TPredicate>
    bool Linq<T>::Any(TPredicate predicate) const
    {
        return std::any_of(data.begin(), data.end(), predicate);
    }

    template<typename T>
    void Linq<T>::Append(const T& element)
    {
        data.push_back(element);
    }

    template<typename T>
    double Linq<T>::Average() const
    {
        return this->Average([](const T& x) { return x; });
    }

    template<typename T>
    template<typename TSelector>
    double Linq<T>::Average(TSelector selector) const
    {
        if (data.empty()) return 0.0;
        double sum = std::accumulate(data.begin(), data.end(), 0.0, [&](double acc, const T& x)
        {
            return acc + static_cast<double>(selector(x));
        });
        return sum / data.size();
    }

    template<typename T>
    template<typename TResult>
    Linq<TResult> Linq<T>::Cast() const
    {
        std::vector<TResult> result;
        std::transform(data.begin(), data.end(), std::back_inserter(result), [](const T& element)
        {
            return static_cast<TResult>(element);
        });
        return Linq<TResult>(result);
    }

    template<typename T>
    void Linq<T>::Clear()
    {
        data.clear();
    }

    template<typename T>
    Linq<T> Linq<T>::Concat(const Linq<T>& other) const
    {
        std::vector<T> result = data;
        result.insert(result.end(), other.data.begin(), other.data.end());
        return Linq<T>(result);
    }

    template<typename T>
    Linq<T> Linq<T>::Concat(const std::vector<T>& other) const
    {
        std::vector<T> result = data;
        result.insert(result.end(), other.begin(), other.end());
        return Linq<T>(result);
    }

    template<typename T>
    bool Linq<T>::Contains(const T& element) const
    {
        return std::find(data.begin(), data.end(), element) != data.end();
    }

    template<typename T>
    void Linq<T>::CopyTo(Linq<T>& other) const
    {
        other.data = data;
    }

    template<typename T>
    size_t Linq<T>::Count() const
    {
        return data.size();
    }

    template<typename T>
    template<typename TPredicate>
    size_t Linq<T>::Count(TPredicate predicate) const
    {
        return std::count_if(data.begin(), data.end(), predicate);
    }

    template<typename T>
    Linq<T> Linq<T>::DefaultIfEmpty(const T& default_value) const
    {
        return data.empty() ? Linq<T>({ default_value }) : *this;
    }

    template<typename T>
    Linq<T> Linq<T>::Distinct() const
    {
        std::vector<T> result;
        std::unordered_set<T> keys;
        for (const auto& element : data)
        {
            if (keys.insert(element).second)
            {
                result.push_back(element);
            }
        }
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TKey>
    Linq<T> Linq<T>::DistinctBy(TKeySelector<TKey> key_selector) const
    {
        std::vector<T> result;
        std::unordered_set<TKey> keys;
        for (const auto& element : data)
        {
            TKey key = key_selector(element);
            if (keys.insert(key).second)
            {
                result.push_back(element);
            }
        }
        return Linq<T>(result);
    }

    template<typename T>
    T Linq<T>::ElementAt(size_t index) const
    {
        if (index >= data.size())
        {
            throw std::out_of_range("Index is out of range.");
        }
        return data[index];
    }

    template<typename T>
    T Linq<T>::ElementAtOrDefault(size_t index, const T& default_value) const
    {
        return index < data.size() ? data[index] : default_value;
    }

    template<typename T>
    Linq<T> Linq<T>::Except(const Linq<T>& other) const
    {
        std::vector<T> result;
        std::unordered_set<T> set(other.data.begin(), other.data.end());
        std::copy_if(data.begin(), data.end(), std::back_inserter(result), [&](const T& element)
        {
            return set.find(element) == set.end();
        });
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TKey>
    Linq<T> Linq<T>::ExceptBy(const Linq<T>& other, TKeySelector<TKey> key_selector) const
    {
        std::vector<T> result;
        std::unordered_set<TKey> keys;
        for (const auto& element : other.data)
        {
            keys.insert(key_selector(element));
        }
        std::copy_if(data.begin(), data.end(), std::back_inserter(result), [&](const T& element)
        {
            return keys.find(key_selector(element)) == keys.end();
        });
        return Linq<T>(result);
    }

    template<typename T>
    T Linq<T>::First() const
    {
        if (data.empty())
        {
            throw std::out_of_range("Sequence contains no elements.");
        }
        return data.front();
    }

    template<typename T>
    template<typename TPredicate>
    T Linq<T>::First(TPredicate predicate) const
    {
        auto it = std::find_if(data.begin(), data.end(), predicate);
        if (it == data.end())
        {
            throw std::logic_error("No matching element found.");
        }
        return *it;
    }

    template<typename T>
    T Linq<T>::FirstOrDefault(const T& default_value) const
    {
        return data.empty() ? default_value : data.front();
    }

    template<typename T>
    template<typename TPredicate>
    T Linq<T>::FirstOrDefault(TPredicate predicate, const T& default_value) const
    {
        auto it = std::find_if(data.begin(), data.end(), predicate);
        return it != data.end() ? *it : default_value;
    }

    template<typename T>
    Linq<T> Linq<T>::Intersect(const Linq<T>& other) const
    {
        std::vector<T> result;
        std::unordered_set<T> set(data.begin(), data.end());
        for (const auto& element : other.data)
        {
            if (set.find(element) != set.end())
            {
                result.push_back(element);
                set.erase(element);
            }
        }
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TKey>
    Linq<T> Linq<T>::IntersectBy(const Linq<T>& other, TKeySelector<TKey> key_selector) const
    {
        std::vector<T> result;
        std::unordered_set<TKey> keys;
        for (const auto& element : other.data)
        {
            keys.insert(key_selector(element));
        }
        std::copy_if(data.begin(), data.end(), std::back_inserter(result), [&](const T& element)
        {
            return keys.find(key_selector(element)) != keys.end();
        });
        return Linq<T>(result);
    }

    template<typename T>
    T Linq<T>::Last() const
    {
        if (data.empty())
        {
            throw std::out_of_range("Sequence contains no elements.");
        }
        return data.back();
    }

    template<typename T>
    template<typename TPredicate>
    T Linq<T>::Last(TPredicate predicate) const
    {
        auto rit = std::find_if(data.rbegin(), data.rend(), predicate);
        if (rit == data.rend())
        {
            throw std::runtime_error("No matching element found.");
        }
        return *rit;
    }

    template<typename T>
    T Linq<T>::LastOrDefault(const T& default_value) const
    {
        return data.empty() ? default_value : data.back();
    }

    template<typename T>
    template<typename TPredicate>
    T Linq<T>::LastOrDefault(TPredicate predicate, const T& default_value) const
    {
        auto rit = std::find_if(data.rbegin(), data.rend(), predicate);
        return rit != data.rend() ? *rit : default_value;
    }

    template<typename T>
    T Linq<T>::Max() const
    {
        if (data.empty())
        {
            throw std::logic_error("Sequence contains no elements.");
        }
        return *std::max_element(data.begin(), data.end());
    }

    template<typename T>
    template<typename TSelector>
    auto Linq<T>::Max(TSelector selector) const -> decltype(selector(data.front()))
    {
        if (data.empty())
        {
            throw std::logic_error("Sequence contains no elements.");
        }
        auto it = std::max_element(data.begin(), data.end(), [&](const T& a, const T& b)
        {
            return selector(a) < selector(b);
        });
        return selector(*it);
    }

    template<typename T>
    template<typename TKey>
    T Linq<T>::MaxBy(TKeySelector<TKey> key_selector) const
    {
        if (data.empty())
        {
            throw std::logic_error("Sequence contains no elements.");
        }
        return *std::max_element(data.begin(), data.end(), [&](const T& a, const T& b)
        {
            return key_selector(a) < key_selector(b);
        });
    }

    template<typename T>
    T Linq<T>::Min() const
    {
        if (data.empty())
        {
            throw std::logic_error("Sequence contains no elements.");
        }
        return *std::min_element(data.begin(), data.end());
    }

    template<typename T>
    template<typename TSelector>
    auto Linq<T>::Min(TSelector selector) const -> decltype(selector(data.front()))
    {
        if (data.empty())
        {
            throw std::logic_error("Sequence contains no elements.");
        }
        auto it = std::min_element(data.begin(), data.end(), [&](const T& a, const T& b)
        {
            return selector(a) < selector(b);
        });
        return selector(*it);
    }

    template<typename T>
    template<typename TKey>
    T Linq<T>::MinBy(TKeySelector<TKey> key_selector) const
    {
        if (data.empty())
        {
            throw std::logic_error("Sequence contains no elements.");
        }
        return *std::min_element(data.begin(), data.end(), [&](const T& a, const T& b)
        {
            return key_selector(a) < key_selector(b);
        });
    }

    template<typename T>
    template<typename TResult>
    Linq<TResult> Linq<T>::OfType() const
    {
        Linq<TResult> result;
        for (const auto& element : data)
        {
            if (auto item = dynamic_cast<const TResult*>(&element))
            {
                result.Append(*item);
            }
        }
        return result;
    }

    template<typename T>
    void Linq<T>::Order()
    {
        std::sort(data.begin(), data.end());
    }

    template<typename T>
    template<typename TKey>
    Linq<T> Linq<T>::OrderBy(TKeySelector<TKey> key_selector) const
    {
        std::vector<T> result = data;
        std::sort(result.begin(), result.end(), [&](const T& a, const T& b)
        {
            return key_selector(a) < key_selector(b);
        });
        return Linq<T>(result);
    }

    template<typename T>
    void Linq<T>::OrderDescending()
    {
        std::sort(data.begin(), data.end(), std::greater<T>());
    }

    template<typename T>
    template<typename TKey>
    Linq<T> Linq<T>::OrderByDescending(TKeySelector<TKey> key_selector) const
    {
        std::vector<T> result = data;
        std::sort(result.begin(), result.end(), [&](const T& a, const T& b)
        {
            return key_selector(a) > key_selector(b);
        });
        return Linq<T>(result);
    }

    template<typename T>
    void Linq<T>::Prepend(const T& element)
    {
        data.insert(data.begin(), element);
    }

    template<typename T>
    void Linq<T>::Remove(const T& element)
    {
        data.erase(std::remove(data.begin(), data.end(), element), data.end());
    }

    template<typename T>
    template<typename TPredicate>
    void Linq<T>::RemoveAll(TPredicate predicate)
    {
        data.erase(std::remove_if(data.begin(), data.end(), predicate), data.end());
    }

    template<typename T>
    Linq<T> Linq<T>::Reverse() const
    {
        std::vector<T> result(data.rbegin(), data.rend());
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TResult>
    Linq<TResult> Linq<T>::Select(TResultSelector<TResult> selector) const
    {
        std::vector<TResult> result;
        std::transform(data.begin(), data.end(), std::back_inserter(result), selector);
        return Linq<TResult>(result);
    }

    template<typename T>
    template<typename TResult>
    Linq<TResult> Linq<T>::SelectMany(TResultSelector<TResult> selector) const
    {
        std::vector<TResult> result;
        for (const auto& element : data)
        {
            Linq<TResult> selected = selector(element);
            result.insert(result.end(), selected.begin(), selected.end());
        }
        return Linq<TResult>(result);
    }

    template<typename T>
    template<typename TCollection, typename TResult>
    Linq<TResult> Linq<T>::SelectMany(
        std::function<Linq<TCollection>(const T&)> collection_selector,
        std::function<TResult(const T&, const TCollection&)> result_selector) const
    {
        std::vector<TResult> result;
        for (const auto& element : data)
        {
            Linq<TCollection> collection = collection_selector(element);
            for (const auto& collection_item : collection)
            {
                result.push_back(result_selector(element, collection_item));
            }
        }
        return Linq<TResult>(result);
    }

    template<typename T>
    T Linq<T>::SingleOrDefault(const T& default_value) const
    {
        if (data.size() == 1)
        {
            return data.front();
        }
        else if (data.empty())
        {
            return default_value;
        }
        throw std::runtime_error("Sequence contains more than one element.");
    }

    template<typename T>
    template<typename TPredicate>
    T Linq<T>::SingleOrDefault(TPredicate predicate, const T& default_value) const
    {
        T result;
        int count = 0;
        for (const auto& element : data)
        {
            if (predicate(element))
            {
                result = element;
                if (++count > 1)
                {
                    throw std::runtime_error("Sequence contains more than one matching element.");
                }
            }
        }
        return count == 1 ? result : default_value;
    }

    template<typename T>
    Linq<T> Linq<T>::Skip(size_t count) const
    {
        if (count >= data.size()) return Linq<T>({});
        return Linq<T>(std::vector<T>(data.begin() + count, data.end()));
    }

    template<typename T>
    Linq<T> Linq<T>::SkipLast(size_t count) const
    {
        if (count >= data.size()) return Linq<T>({});
        return Linq<T>(std::vector<T>(data.begin(), data.end() - count));
    }

    template<typename T>
    template<typename TPredicate>
    Linq<T> Linq<T>::SkipWhile(TPredicate predicate) const
    {
        auto it = std::find_if_not(data.begin(), data.end(), predicate);
        if (it == data.end()) return Linq<T>({});
        return Linq<T>(std::vector<T>(it, data.end()));
    }

    template<typename T>
    T Linq<T>::Sum() const
    {
        return std::accumulate(data.begin(), data.end(), T());
    }

    template<typename T>
    template<typename TResult>
    TResult Linq<T>::Sum(TResultSelector<TResult> selector) const
    {
        return std::accumulate(data.begin(), data.end(), TResult(), [&](TResult acc, const T& element)
        {
            return acc + selector(element);
        });
    }

    template<typename T>
    Linq<T> Linq<T>::Take(size_t count) const
    {
        if (count >= data.size()) return *this;
        return Linq<T>(std::vector<T>(data.begin(), data.begin() + count));
    }

    template<typename T>
    Linq<T> Linq<T>::TakeLast(size_t count) const
    {
        if (count >= data.size()) return *this;
        return Linq<T>(std::vector<T>(data.end() - count, data.end()));
    }

    template<typename T>
    template<typename TPredicate>
    Linq<T> Linq<T>::TakeWhile(TPredicate predicate) const
    {
        auto it = std::find_if_not(data.begin(), data.end(), predicate);
        if (it == data.end()) return *this;
        return Linq<T>(std::vector<T>(data.begin(), it));
    }

    template<typename T>
    std::vector<T> Linq<T>::ToArray() const
    {
        return data;
    }

    template<typename T>
    Linq<T> Linq<T>::Union(const Linq<T>& other) const
    {
        std::vector<T> raw(data.begin(), data.end());
        raw.insert(raw.end(), other.data.begin(), other.data.end());
        std::unordered_set<T> result(raw.begin(), raw.end());
        return Linq<T>(std::vector<T>(result.begin(), result.end()));
    }

    template<typename T>
    template<typename TKey>
    Linq<T> Linq<T>::UnionBy(const Linq<T>& other, TKeySelector<TKey> key_selector) const
    {
        std::vector<T> result;
        std::unordered_set<TKey> keys;
        std::vector<T> raw(data.begin(), data.end());
        raw.insert(raw.end(), other.data.begin(), other.data.end());
        for (const auto& element : raw)
        {
            TKey key = key_selector(element);
            if (keys.insert(key).second)
            {
                result.push_back(element);
            }
        }
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TPredicate>
    Linq<T> Linq<T>::Where(TPredicate predicate) const
    {
        std::vector<T> result;
        std::copy_if(data.begin(), data.end(), std::back_inserter(result), predicate);
        return Linq<T>(result);
    }

    template<typename T>
    template<typename U>
    Linq<std::pair<T, U>> Linq<T>::Zip(const Linq<U>& other) const
    {
        std::vector<std::pair<T, U>> result;
        size_t size = data.size();
        result.reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            result.emplace_back(data[i], other.data[i]);
        }
        return Linq<std::pair<T, U>>(result);
    }

    template<typename T>
    template<typename U, typename TResult>
    Linq<TResult> Linq<T>::Zip(const Linq<U>& other,
        std::function<TResult(const T&, const U&)> result_selector) const
    {
        std::vector<TResult> result;
        size_t size = data.size();
        result.reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            result.push_back(result_selector(data[i], other.data[i]));
        }
        return Linq<TResult>(result);
    }

    template<typename T>
    typename std::vector<T>::const_iterator Linq<T>::begin() const
    {
        return data.begin();
    }

    template<typename T>
    typename std::vector<T>::const_iterator Linq<T>::end() const
    {
        return data.end();
    }
}
