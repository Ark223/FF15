
#ifndef LINQ_H
#define LINQ_H

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <numeric>
#include <regex>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace Evade
{
    template<typename T>
    class Linq
    {
        template<typename TKey>
        using TKeySelector = std::function<TKey(const T&)>;

        template<typename TResult>
        using TResultSelector = std::function<TResult(const T&)>;

        private:
            std::vector<T> data;

        public:
            Linq() = default;
            explicit Linq(const std::vector<T>& input);
            explicit Linq(std::vector<T>&& input);
            Linq(std::initializer_list<T> init_list);

            void AddRange(const Linq<T>& other);

            void AddRange(const std::vector<T>& other);

            void AddRange(std::initializer_list<T> other);

            template<typename TReducer>
            auto Aggregate(TReducer reducer) const;

            template<typename TAccumulate, typename TReduce>
            TAccumulate Aggregate(TAccumulate seed, TReduce reducer) const;

            template<typename TAccumulate, typename TReduce, typename TSelector>
            TAccumulate Aggregate(TAccumulate seed, TReduce reducer, TSelector selector) const;

            template<typename TPredicate>
            bool All(TPredicate predicate) const;

            bool Any() const;

            template<typename TPredicate>
            bool Any(TPredicate predicate) const;

            void Append(const T& element);

            double Average() const;

            template<typename TSelector>
            double Average(TSelector selector) const;

            template<typename TResult>
            Linq<TResult> Cast() const;

            void Clear();

            Linq Concat(const Linq<T>& other) const;

            Linq Concat(const std::vector<T>& other) const;

            Linq Concat(std::initializer_list<T> other) const;

            bool Contains(const T& element) const;

            void CopyTo(Linq<T>& other) const;

            size_t Count() const;

            template<typename TPredicate>
            size_t Count(TPredicate predicate) const;

            Linq<T> DefaultIfEmpty(const T& default_value = T()) const;

            Linq Distinct() const;

            template<typename TKey>
            Linq DistinctBy(TKeySelector<TKey> key_selector) const;

            T ElementAt(size_t index) const;

            T ElementAtOrDefault(size_t index, const T& default_value = T()) const;

            Linq Except(const Linq<T>& other) const;

            template<typename TKey>
            Linq ExceptBy(const Linq<T>& other, TKeySelector<TKey> key_selector) const;

            T First() const;

            template<typename TPredicate>
            T First(TPredicate predicate) const;

            T FirstOrDefault(const T& default_value = T()) const;

            template<typename TPredicate>
            T FirstOrDefault(TPredicate predicate, const T& default_value = T()) const;
            
            template<typename TAction>
            void ForEach(TAction action) const;

            int IndexOf(T item, int index = -1, int count = -1) const;

            Linq Intersect(const Linq<T>& other) const;

            template<typename TKey>
            Linq IntersectBy(const Linq<T>& other, TKeySelector<TKey> key_selector) const;

            T Last() const;

            template<typename TPredicate>
            T Last(TPredicate predicate) const;

            int LastIndexOf(T item, int index = -1, int count = -1) const;

            T LastOrDefault(const T& default_value = T()) const;

            template<typename TPredicate>
            T LastOrDefault(TPredicate predicate, const T& default_value = T()) const;

            T Max() const;

            template<typename TSelector>
            auto Max(TSelector selector) const -> decltype(selector(data.front()));

            template<typename TKey>
            T MaxBy(TKeySelector<TKey> key_selector) const;

            T Min() const;

            template<typename TSelector>
            auto Min(TSelector selector) const -> decltype(selector(data.front()));

            template<typename TKey>
            T MinBy(TKeySelector<TKey> key_selector) const;

            template<typename TResult>
            Linq<TResult> OfType() const;

            void Order();

            template<typename TKey>
            Linq OrderBy(TKeySelector<TKey> key_selector) const;

            void OrderDescending();

            template<typename TKey>
            Linq OrderByDescending(TKeySelector<TKey> key_selector) const;

            void Prepend(const T& element);

            void Remove(const T& element);

            template<typename TPredicate>
            void RemoveAll(TPredicate predicate);

            void RemoveAt(int index);

            void RemoveRange(int index, int count);

            Linq<T> Reverse() const;

            template<typename TResult>
            Linq<TResult> Select(TResultSelector<TResult> selector) const;

            template<typename TResult>
            Linq<TResult> SelectMany(TResultSelector<TResult> selector) const;

            template<typename TCollection, typename TResult>
            Linq<TResult> SelectMany(
                std::function<Linq<TCollection>(const T&)> collection_selector,
                std::function<TResult(const T&, const TCollection&)> result_selector) const;

            T SingleOrDefault(const T& default_value = T()) const;

            template<typename TPredicate>
            T SingleOrDefault(TPredicate predicate, const T& default_value = T()) const;

            Linq<T> Skip(size_t count) const;

            Linq<T> SkipLast(size_t count) const;

            template<typename TPredicate>
            Linq<T> SkipWhile(TPredicate predicate) const;

            T Sum() const;

            template<typename TResult>
            TResult Sum(TResultSelector<TResult> selector) const;

            Linq<T> Take(size_t count) const;

            Linq<T> TakeLast(size_t count) const;

            template<typename TPredicate>
            Linq<T> TakeWhile(TPredicate predicate) const;

            std::vector<T> ToArray() const;

            Linq<T> Union(const Linq<T>& other) const;

            template<typename TKey>
            Linq<T> UnionBy(const Linq<T>& other, TKeySelector<TKey> key_selector) const;

            template<typename TPredicate>
            Linq<T> Where(TPredicate predicate) const;

            template<typename U>
            Linq<std::pair<T, U>> Zip(const Linq<U>&) const;

            template<typename U, typename TResult>
            Linq<TResult> Zip(const Linq<U>&, std::function<TResult(const T&, const U&)>) const;

            typename std::vector<T>::const_iterator begin() const;
            typename std::vector<T>::const_iterator end() const;

            typename std::vector<T>::iterator begin();
            typename std::vector<T>::iterator end();
    };
}

#endif // LINQ_H
