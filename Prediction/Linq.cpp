
#include "Linq.h"

namespace IPrediction
{
    template<typename T>
    Linq<T>::Linq(std::initializer_list<T> input) : data(input) {}

    template<typename T>
    Linq<T>::Linq(const std::vector<T>& input) : data(input) {}

    template<typename T>
    Linq<T>::Linq(std::vector<T>&& input) : data(input) {}

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
    void Linq<T>::AddRange(std::initializer_list<T> other)
    {
        data.insert(data.end(), other.begin(), other.end());
    }

    template<typename T>
    template<typename TReducer>
    auto Linq<T>::Aggregate(TReducer reducer) const
    {
        auto result = data.front();
        for (size_t i = 1; i < data.size(); ++i)
        {
            result = reducer(result, data[i]);
        }
        return result;
    }

    template<typename T>
    template<typename TAccumulate, typename TReduce>
    TAccumulate Linq<T>::Aggregate(TAccumulate seed, TReduce reducer) const
    {
        TAccumulate result = seed;
        for (const auto& element : data)
        {
            result = reducer(result, element);
        }
        return result;
    }

    template<typename T>
    template<typename TAccumulate, typename TReduce, typename TSelector>
    TAccumulate Linq<T>::Aggregate(TAccumulate seed, TReduce reducer, TSelector selector) const
    {
        TAccumulate result = seed;
        for (const auto& element : data)
        {
            result = reducer(result, selector(element));
        }
        return result;
    }

    template<typename T>
    template<typename TPredicate>
    bool Linq<T>::All(TPredicate predicate) const
    {
        for (const auto& element : data)
        {
            if (!predicate(element))
            {
                return false;
            }
        }
        return true;
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
        for (const auto& element : data)
        {
            if (predicate(element))
            {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    void Linq<T>::Append(const T& element)
    {
        data.push_back(element);
    }

    template<typename T>
    double Linq<T>::Average() const
    {
        double sum = 0.0;
        if (data.empty()) return sum;
        for (const auto& element : data)
        {
            sum += (double)(element);
        }
        return sum / data.size();
    }

    template<typename T>
    template<typename TSelector>
    double Linq<T>::Average(TSelector selector) const
    {
        double sum = 0.0;
        if (data.empty()) return sum;
        for (const auto& element : data)
        {
            sum += (double)(selector(element));
        }
        return sum / data.size();
    }

    template<typename T>
    template<typename TResult>
    Linq<TResult> Linq<T>::Cast() const
    {
        std::vector<TResult> result;
        result.reserve(data.size());
        for (const auto& element : data)
        {
            result.push_back((TResult)(element));
        }
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
    Linq<T> Linq<T>::Concat(std::initializer_list<T> other) const
    {
        std::vector<T> result = data;
        result.insert(result.end(), other.begin(), other.end());
        return Linq<T>(result);
    }

    template<typename T>
    bool Linq<T>::Contains(const T& element) const
    {
        for (const auto& item : data)
        {
            if (item == element)
            {
                return true;
            }
        }
        return false;
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
        size_t count = 0;
        for (const auto& element : data)
        {
            if (predicate(element))
            {
                ++count;
            }
        }
        return count;
    }

    template<typename T>
    Linq<T> Linq<T>::DefaultIfEmpty(const T& default_value) const
    {
        return data.empty() ? Linq<T>({ default_value }) : *this;
    }

    template<typename T>
    void Linq<T>::Delete(const T& element)
    {
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            if (*it == element)
            {
                if constexpr (std::is_pointer<T>::value)
                {
                    delete* it;
                }
                data.erase(it);
                break;
            }
        }
    }

    template<typename T>
    template<typename TPredicate>
    void Linq<T>::DeleteAll(TPredicate predicate)
    {
        for (auto it = data.begin(); it != data.end();)
        {
            if (predicate(*it))
            {
                if constexpr (std::is_pointer<T>::value)
                {
                    delete* it;
                }
                it = data.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    template<typename T>
    Linq<T> Linq<T>::Distinct() const
    {
        std::vector<T> result;
        std::unordered_map<T, bool> keys;
        for (const auto& element : data)
        {
            if (keys.find(element) == keys.end())
            {
                keys[element] = true;
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
        std::unordered_map<TKey, bool> keys;
        for (const auto& element : data)
        {
            TKey key = key_selector(element);
            if (keys.find(key) == keys.end())
            {
                keys[key] = true;
                result.push_back(element);
            }
        }
        return Linq<T>(result);
    }

    template<typename T>
    T Linq<T>::ElementAt(size_t index) const
    {
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
        std::unordered_map<T, bool> set;
        for (const auto& element : other.data)
        {
            set[element] = true;
        }
        for (const auto& element : data)
        {
            if (set.find(element) == set.end())
            {
                result.push_back(element);
            }
        }
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TKey>
    Linq<T> Linq<T>::ExceptBy(const Linq<T>& other, TKeySelector<TKey> key_selector) const
    {
        std::vector<T> result;
        std::unordered_map<TKey, bool> keys;
        for (const auto& element : other.data)
        {
            keys[key_selector(element)] = true;
        }
        for (const auto& element : data)
        {
            TKey key = key_selector(element);
            if (keys.find(key) == keys.end())
            {
                result.push_back(element);
            }
        }
        return Linq<T>(result);
    }

    template<typename T>
    T Linq<T>::First() const
    {
        return data.front();
    }

    template<typename T>
    template<typename TPredicate>
    T Linq<T>::First(TPredicate predicate) const
    {
        for (const auto& element : data)
        {
            if (predicate(element))
            {
                return element;
            }
        }
        return T();
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
        for (const auto& element : data)
        {
            if (predicate(element))
            {
                return element;
            }
        }
        return default_value;
    }

    template<typename T>
    template<typename TAction>
    void Linq<T>::ForEach(TAction action) const
    {
        for (const auto& element : data)
        {
            action(element);
        }
    }

    template<typename T>
    int Linq<T>::IndexOf(T item, int index, int count) const
    {
        size_t size = data.size();
        if (index == -1) index = 0;
        if (count == -1) count = size - index;

        for (int i = index; i < index + count && i < size; ++i)
        {
            if (data[i] == item) return i;
        }
        return -1;
    }

    template<typename T>
    Linq<T> Linq<T>::Intersect(const Linq<T>& other) const
    {
        std::vector<T> result;
        std::unordered_map<T, bool> set;
        for (const auto& element : data)
        {
            set[element] = true;
        }
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
        std::unordered_map<TKey, bool> keys;
        for (const auto& element : other.data)
        {
            keys[key_selector(element)] = true;
        }
        for (const auto& element : data)
        {
            TKey key = key_selector(element);
            if (keys.find(key) != keys.end())
            {
                result.push_back(element);
            }
        }
        return Linq<T>(result);
    }

    template<typename T>
    T Linq<T>::Last() const
    {
        return data.back();
    }

    template<typename T>
    template<typename TPredicate>
    T Linq<T>::Last(TPredicate predicate) const
    {
        size_t size = data.size();
        for (size_t i = size - 1; i >= 0; --i)
        {
            if (predicate(data[i]))
            {
                return data[i];
            }
        }
        return T();
    }

    template<typename T>
    int Linq<T>::LastIndexOf(T item, int index, int count) const
    {
        size_t size = data.size();
        if (index == -1) index = size - 1;
        if (count == -1) count = index + 1;

        for (int i = index; i >= index - count + 1 && i >= 0; --i)
        {
            if (data[i] == item) return i;
        }
        return -1;
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
        size_t size = data.size();
        for (size_t i = size - 1; i >= 0; --i)
        {
            if (predicate(data[i]))
            {
                return data[i];
            }
        }
        return default_value;
    }

    template<typename T>
    T Linq<T>::Max() const
    {
        T max_value = data.front();
        for (size_t i = 1; i < data.size(); ++i)
        {
            if (data[i] > max_value)
            {
                max_value = data[i];
            }
        }
        return max_value;
    }

    template<typename T>
    template<typename TSelector>
    auto Linq<T>::Max(TSelector selector) const -> decltype(selector(data.front()))
    {
        auto max_element = data.front();
        for (size_t i = 1; i < data.size(); ++i)
        {
            if (selector(data[i]) > selector(max_element))
            {
                max_element = data[i];
            }
        }
        return selector(max_element);
    }

    template<typename T>
    template<typename TKey>
    T Linq<T>::MaxBy(TKeySelector<TKey> key_selector) const
    {
        T max_element = data.front();
        for (size_t i = 1; i < data.size(); ++i)
        {
            if (key_selector(data[i]) > key_selector(max_element))
            {
                max_element = data[i];
            }
        }
        return max_element;
    }

    template<typename T>
    T Linq<T>::Min() const
    {
        T min_value = data.front();
        for (size_t i = 1; i < data.size(); ++i)
        {
            if (data[i] < min_value)
            {
                min_value = data[i];
            }
        }
        return min_value;
    }

    template<typename T>
    template<typename TSelector>
    auto Linq<T>::Min(TSelector selector) const -> decltype(selector(data.front()))
    {
        auto min_element = data.front();
        for (size_t i = 1; i < data.size(); ++i)
        {
            if (selector(data[i]) < selector(min_element))
            {
                min_element = data[i];
            }
        }
        return selector(min_element);
    }

    template<typename T>
    template<typename TKey>
    T Linq<T>::MinBy(TKeySelector<TKey> key_selector) const
    {
        T min_element = data.front();
        for (size_t i = 1; i < data.size(); ++i)
        {
            if (key_selector(data[i]) < key_selector(min_element))
            {
                min_element = data[i];
            }
        }
        return min_element;
    }

    template<typename T>
    template<typename TResult>
    Linq<TResult> Linq<T>::OfType() const
    {
        Linq<TResult> result;
        result.reserve(data.size());
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
        comparer = [](const T& a, const T& b)
        {
            return a < b;
        };
        MergeSort((int)(data.size()));
    }

    template<typename T>
    template<typename TKey>
    Linq<T> Linq<T>::OrderBy(TKeySelector<TKey> key_selector) const
    {
        Linq<T> result(data);
        result.comparer = [key_selector](const T& a, const T& b)
        {
            return key_selector(a) < key_selector(b);
        };
        result.MergeSort((int)(data.size()));
        return result;
    }

    template<typename T>
    void Linq<T>::OrderDescending()
    {
        comparer = [](const T& a, const T& b)
        {
            return a > b;
        };
        MergeSort((int)(data.size()));
    }

    template<typename T>
    template<typename TKey>
    Linq<T> Linq<T>::OrderByDescending(TKeySelector<TKey> key_selector) const
    {
        Linq<T> result(data);
        result.comparer = [key_selector](const T& a, const T& b)
        {
            return key_selector(a) > key_selector(b);
        };
        result.MergeSort((int)(data.size()));
        return result;
    }

    template<typename T>
    void Linq<T>::Prepend(const T& element)
    {
        data.insert(data.begin(), element);
    }

    template<typename T>
    void Linq<T>::Remove(const T& element)
    {
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            if (*it == element) return (void)data.erase(it);
        }
    }

    template<typename T>
    template<typename TPredicate>
    void Linq<T>::RemoveAll(TPredicate predicate)
    {
        for (auto it = data.begin(); it != data.end();)
        {
            if (predicate(*it))
            {
                it = data.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    template<typename T>
    void Linq<T>::RemoveAt(size_t index)
    {
        data.erase(data.begin() + index);
    }

    template<typename T>
    void Linq<T>::RemoveRange(size_t index, size_t count)
    {
        data.erase(data.begin() + index, data.begin() + index + count);
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
        result.reserve(data.size());
        for (const auto& element : data)
        {
            result.push_back(selector(element));
        }
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
        return data.size() == 1 ? data.front() : default_value;
    }

    template<typename T>
    template<typename TPredicate>
    T Linq<T>::SingleOrDefault(TPredicate predicate, const T& default_value) const
    {
        T* result = nullptr;
        for (const auto& element : data)
        {
            if (predicate(element))
            {
                if (result != nullptr)
                {
                    return default_value;
                }
                result = &element;
            }
        }
        return result ? *result : default_value;
    }

    template<typename T>
    Linq<T> Linq<T>::Skip(size_t count) const
    {
        std::vector<T> result;
        size_t size = data.size();
        if (count >= size)
        {
            return Linq<T>(result);
        }
        result.reserve(size - count);
        for (size_t i = count; i < size; ++i)
        {
            result.push_back(data[i]);
        }
        return Linq<T>(result);
    }

    template<typename T>
    Linq<T> Linq<T>::SkipLast(size_t count) const
    {
        std::vector<T> result;
        size_t size = data.size();
        if (count >= size)
        {
            return Linq<T>(result);
        }
        result.reserve(size - count);
        for (size_t i = 0; i < size - count; ++i)
        {
            result.push_back(data[i]);
        }
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TPredicate>
    Linq<T> Linq<T>::SkipWhile(TPredicate predicate) const
    {
        size_t i = 0;
        std::vector<T> result;
        size_t size = data.size();
        while (i < size && predicate(data[i]))
        {
            ++i;
        }
        for (; i < size; ++i)
        {
            result.push_back(data[i]);
        }
        return Linq<T>(result);
    }

    template<typename T>
    T Linq<T>::Sum() const
    {
        T result = T();
        for (const auto& element : data)
        {
            result += element;
        }
        return result;
    }

    template<typename T>
    template<typename TResult>
    TResult Linq<T>::Sum(TResultSelector<TResult> selector) const
    {
        TResult result = TResult();
        for (const auto& element : data)
        {
            result += selector(element);
        }
        return result;
    }

    template<typename T>
    Linq<T> Linq<T>::Take(size_t count) const
    {
        std::vector<T> result;
        size_t size = data.size();
        if (count >= size)
        {
            return *this;
        }
        result.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            result.push_back(data[i]);
        }
        return Linq<T>(result);
    }

    template<typename T>
    Linq<T> Linq<T>::TakeLast(size_t count) const
    {
        std::vector<T> result;
        size_t size = data.size();
        if (count >= size)
        {
            return *this;
        }
        result.reserve(count);
        for (size_t i = size - count; i < size; ++i)
        {
            result.push_back(data[i]);
        }
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TPredicate>
    Linq<T> Linq<T>::TakeWhile(TPredicate predicate) const
    {
        std::vector<T> result;
        for (const auto& element : data)
        {
            if (!predicate(element))
            {
                break;
            }
            result.push_back(element);
        }
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TKey>
    Linq<T>& Linq<T>::ThenBy(TKeySelector<TKey> key_selector)
    {
        auto prev_comp = comparer;
        comparer = [prev_comp, key_selector](const T& a, const T& b)
        {
            if (prev_comp && prev_comp(a, b)) return true;
            if (prev_comp && prev_comp(b, a)) return false;
            return key_selector(a) < key_selector(b);
        };
        MergeSort((int)(data.size()));
        return *this;
    }

    template<typename T>
    template<typename TKey>
    Linq<T>& Linq<T>::ThenByDescending(TKeySelector<TKey> key_selector)
    {
        auto prev_comp = comparer;
        comparer = [prev_comp, key_selector](const T& a, const T& b)
        {
            if (prev_comp && prev_comp(a, b)) return true;
            if (prev_comp && prev_comp(b, a)) return false;
            return key_selector(a) > key_selector(b);
        };
        MergeSort((int)(data.size()));
        return *this;
    }

    template<typename T>
    std::vector<T>& Linq<T>::ToArray()
    {
        return data;
    }

    template<typename T>
    Linq<T> Linq<T>::Union(const Linq<T>& other) const
    {
        std::vector<T> result = data;
        for (const auto& element : other.data)
        {
            bool found = false;
            for (const auto& item : result)
            {
                if (item == element)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                result.push_back(element);
            }
        }
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TKey>
    Linq<T> Linq<T>::UnionBy(const Linq<T>& other, TKeySelector<TKey> key_selector) const
    {
        std::vector<TKey> keys;
        std::vector<T> result = data;
        for (const auto& element : data)
        {
            keys.push_back(key_selector(element));
        }
        for (const auto& element : other.data)
        {
            bool found = false;
            TKey key = key_selector(element);
            for (const auto& item : keys)
            {
                if (item == key)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                result.push_back(element);
                keys.push_back(key);
            }
        }
        return Linq<T>(result);
    }

    template<typename T>
    template<typename TPredicate>
    Linq<T> Linq<T>::Where(TPredicate predicate) const
    {
        std::vector<T> result;
        for (const auto& element : data)
        {
            if (predicate(element))
            {
                result.push_back(element);
            }
        }
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
            result.push_back({ data[i], other.data[i] });
        }
        return Linq<std::pair<T, U>>(result);
    }

    template<typename T>
    template<typename U, typename TResult>
    Linq<TResult> Linq<T>::Zip(const Linq<U>& other, TZipSelector<TResult, U> result_selector) const
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

    // Utilities

    template<typename T>
    void Linq<T>::InsertionSort(int left, int right)
    {
        for (int i = left + 1; i <= right; i++)
        {
            for (int j = i; j > left && comparer(data[j], data[j - 1]); j--)
            {
                SwapValues(data[j], data[j - 1]);
            }
        }
    }

    template<typename T>
    void Linq<T>::Merge(int left, int mid, int right)
    {
        int len = mid - left + 1;
        for (int i = 0; i < len; i++)
        {
            buffer[i] = data[left + i];
        }

        int i = 0, j = mid + 1, k = left;
        while (i < len && j <= right)
        {
            if (!comparer(data[j], buffer[i]))
            {
                data[k++] = buffer[i++];
            }
            else
            {
                data[k++] = data[j++];
            }
        }
        while (i < len)
        {
            data[k++] = buffer[i++];
        }
    }

    template<typename T>
    void Linq<T>::MergeSort(int size)
    {
        const int run = 32;
        buffer = std::vector<T>(size);

        auto min = [](int a, int b)
        {
            return a < b ? a : b;
        };
        for (int i = 0; i < size; i += run)
        {
            int right = min(i + run - 1, size - 1);
            InsertionSort(i, right);
        }
        for (int i = run; i < size; i *= 2)
        {
            for (int left = 0; left < size; left += 2 * i)
            {
                int mid = left + i - 1;
                int right = min(left + 2 * i - 1, size - 1);
                if (mid < right) Merge(left, mid, right);
            }
        }
    }

    template<typename T>
    void Linq<T>::SwapValues(T& a, T& b)
    {
        T temp = static_cast<T&&>(a);
        a = static_cast<T&&>(b);
        b = static_cast<T&&>(temp);
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

    template<typename T>
    typename std::vector<T>::iterator Linq<T>::begin()
    {
        return data.begin();
    }

    template<typename T>
    typename std::vector<T>::iterator Linq<T>::end()
    {
        return data.end();
    }
}
