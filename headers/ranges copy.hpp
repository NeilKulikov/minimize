#ifndef RANGES
#define RANGES

#include <utility>
#include <iterator>
#include <algorithm>
#include <functional>
#include <type_traits>

namespace minimize{
    namespace ranges{
        namespace iters{
            template<typename it>
            class iterator : public std::iterator<std::forward_iterator_tag, typename it::value_type>{
                public:
                    using ty = typename it::value_type;
                    using re = typename it::reference;
                    using value_type = ty;
                protected:
                    const it _begin;
                    const it _current;
                public:
                    iterator(const it begin, const it current):
                        _begin(begin),
                        _current(current)
                        {};
                    iterator(iterator<it>& prev_it):
                        iterator(prev_it._begin, prev_it._current)
                        {};
                    iterator<it>& operator++(void){
                        _current = std::next(_current);
                        return *this;
                    };
                    iterator<it> operator++(int){
                        iterator ret_val(*this);
                        operator++();
                        return ret_val;
                    };
                    bool operator==(const iterator<it>& oth) const{
                        return 
                            (_current == oth._current) 
                                && 
                            (_begin == oth._begin);
                    };
                    bool operator!=(const iterator<it>& oth) const{
                        return !operator==(oth);
                    };
                    re operator*(void){
                        return *_current;
                    };
                    const re operator*(void) const{
                        return *_current;
                    };
                    std::size_t num(void) const{
                        return std::distance(_begin, _current);
                    };
            };
            template<typename it>
            class subs_iterator : public iterator<it>{
                public:
                    using ty = typename iterator<it>::ty;
                    using subs = typename std::pair<std::size_t, ty>;
                    using value_type = ty;
                protected:
                    const subs _body;
                public:
                    subs_iterator(const it begin, const it current, const subs body):
                        iterator<it>(begin, current),
                        _body(body)
                        {};
                    subs_iterator(subs_iterator& prev_it):
                        subs_iterator(prev_it._begin, prev_it._end, prev_it._body)
                        {};
                    ty operator*(void) const{
                        const bool out = iterator<it>::num() == _body.first;
                        return out ?
                                _body.second :
                                iterator<it>::operator*();
                    };
            };

            template<typename it1, typename it2, typename op>
            class bop_iterator: public std::iterator<std::forward_iterator_tag, 
                    typename std::result_of<op(typename it1::value_type, typename it2::value_type)>::type >{
                public:
                    using ty1 = typename it1::value_type;
                    using ty2 = typename it2::value_type;
                    using ty = typename std::result_of<op(ty1, ty2)>::type;
                    using value_type = ty;
                protected:
                    const op _oper;
                protected:
                    it1 _cur1;
                    it2 _cur2;
                public:
                    bop_iterator(const op& oper, const it1& iter1, const it2& iter2):
                        _oper(oper),
                        _cur1(iter1),
                        _cur2(iter2)
                        {};
                    ty operator*(void) const{
                        return _oper(*_cur1, *_cur2);
                    };
                    ty operator++(void){
                        _cur1 = std::next(_cur1);
                        _cur2 = std::next(_cur2);
                        return operator*();
                    };
                    ty operator++(int){
                        ty ret_val = operator*();
                        operator++();
                        return std::move(ret_val);
                    };
                    bool operator==(const bop_iterator<it1, it2, op>& oth){
                        return (_cur1 == oth._cur1) && (_cur2 == oth._cur2);
                    };
                    bool operator!=(const bop_iterator<it1, it2, op>& oth){
                        return !operator==(oth);
                    };
            };
        };
        template<typename T, typename it>
        class range_proto{
            public:
                using co = T;
                using ty = typename it::value_type;
                using re = typename it::reference;
                using iterator = typename iters::iterator<it>;
                using value_type = ty;
            public:
                const it _begin, _end;
            public:
                range_proto(const it begin, const it end):
                    _begin(begin),
                    _end(end)
                    {};
                range_proto(T& cont):
                    range_proto(cont.begin(), cont.end())
                    {};
                std::size_t size(void) const{
                    return std::distance(_begin, _end);
                };
            public:
                iterator begin(void) const{
                    return iterator(_begin, _begin);
                };
                iterator end(void) const{
                    return iterator(_begin, _end);
                };
                iterator iterator_at(const std::size_t num){
                    return iterator(_begin, std::next(_begin, num));
                };
                iterator iterator_at(const std::size_t num) const{
                    return iterator(_begin, std::next(_begin, num));
                };
                re at(const size_t num){
                    return *std::next(_begin, num);
                };
                const re at(const size_t num) const{
                    return *std::next(_begin, num);
                };
        };
        template<typename T>
        class const_range : public range_proto<T, typename T::const_iterator>{
            public:
                using co = T;
                using ty = typename T::value_type;
                using it = typename T::const_iterator;
                using pr = range_proto<T, it>;
                using const_iterator = typename pr::iterator;
            public:
                const_range(const it begin, const it end):
                    range_proto<T, it>(begin, end)
                    {};
                const_range(const T& cont):
                    const_range<T>(cont.cbegin(), cont.cend())
                    {};
                const_iterator cbegin(void) const{
                    return pr::begin();
                };
                const_iterator cend(void) const{
                    return pr::end();
                };
        };
        template<typename T>
        class range : public range_proto<T, typename T::iterator>{
            public:
                using co = T;
                using ty = typename T::value_type;
                using it = typename T::iterator;
            public:
                range(const it begin, const it end):
                    range_proto<T, it>(begin, end)
                    {};
                range(T& cont):
                    range<T>(cont.begin(), cont.end())
                    {};
        };  
        template<typename T> 
        class subs_range : public const_range<T>{
            public:
                using co = T;
                using cr = const_range<T>;
                using ty = typename cr::ty;
                using it = typename cr::it;
                using re = typename cr::re;
                using subs = typename std::pair<std::size_t, ty>;
                using iterator = typename iters::subs_iterator<it>;
            protected:
                const subs _body;
            public:
                subs_range(const_range<T> cr, const subs body):
                    const_range<T>(cr),
                    _body(body)
                    {};
                subs_range(const T& cont, const subs body):
                    subs_range(const_range<T>(cont), body)
                    {};
            public:
                iterator begin(void) const{
                    return iterator(cr::_begin, cr::_begin, _body);
                };
                iterator end(void) const{
                    return iterator(cr::_begin, cr::_end, _body);
                };
                iterator iterator_at(const std::size_t num) const{
                    return iterator(cr::_begin, std::next(cr::_begin, num), _body);
                };
                re at(const size_t num){
                    return (num == _body.first) ?
                            _body.second :
                            const_range<T>::at(num);
                };
                const re at(const size_t num) const{
                    return (num == _body.first) ?
                            _body.second :
                            const_range<T>::at(num);
                };
        }; 

        template<typename T1, typename T2, typename op>
        class bop_range{
            public:
                using it1 = typename T1::const_iterator;
                using it2 = typename T2::const_iterator;
                using ty1 = typename T1::value_type;
                using ty2 = typename T2::value_type;
                using ty = typename std::result_of<op(ty1, ty2)>::type;
                using iterator = typename iters::bop_iterator<it1, it2, op>;
                using const_iterator = iterator;
                using value_type = ty;
            protected:
                const op _oper;
                const it1 _beg1, _end1;
                const it2 _beg2, _end2;
                const std::size_t _size;
            public:
                bop_range(const op& oper, T1& c1, T2& c2):
                    _oper(oper),
                    _beg1(c1.begin()), _end1(c1.end()),
                    _beg2(c2.begin()), _end2(c2.end()),
                    _size(std::distance(_beg1, _end1))
                    {
                        if(std::distance(_beg1, _end1) != std::distance(_beg2, _end2))
                            throw std::length_error("cont1 should have same length with cont2");
                    };
                bop_range(const op& oper, const T1& c1, const T2& c2):
                    _oper(oper),
                    _beg1(c1.cbegin()), _end1(c1.cend()),
                    _beg2(c2.cbegin()), _end2(c2.cend()),
                    _size(std::distance(_beg1, _end1))
                    {
                        if(std::distance(_beg1, _end1) != std::distance(_beg2, _end2))
                            throw std::length_error("cont1 should have same length with cont2");
                    };
                iterator cbegin(void) const{
                    return {_beg1, _beg2};
                };
                iterator cend(void) const{
                    return {_beg1, _beg2};
                };
                iterator begin(void) const{ return begin();};
                iterator end(void) const{ return cend(); };
            protected:
                it1 iterator1_at(const std::size_t i) const{
                    return std::next(_beg1, i);
                };
                it2 iterator2_at(const std::size_t i) const{
                    return std::next(_beg2, i);
                };
            public:
                iterator iterator_at(const std::size_t i) const{
                    return {_oper, iterator1_at(i), iterator2_at(i)};
                };
                ty at(const std::size_t i) const{
                    return *iterator_at(i);
                };
                std::size_t size(void) const{
                    return _size;
                };
        };

        namespace ops{
            template<typename T>
            using bop = const typename std::function<T(T, T)>&;
            template<typename T1, typename T2, typename R>
            using rbop = const typename std::function<R(T1, T2)>&;

            template<typename T1, typename T2>
            constexpr auto plus = [](T1 a, T2 b) -> decltype(a + b) { return a + b; };
            template<typename T1, typename T2>
            auto sum(const T1& c1, const T2& c2){
                using ty1 = typename T1::value_type;
                using ty2 = typename T2::value_type;
                using bop = decltype(plus<ty1, ty2>);
                return bop_range<T1, T2, bop>(plus<ty1, ty2>, c1, c2);
            };

            template<typename T1, typename T2>
            constexpr auto minus = [](T1 a, T2 b) -> decltype(a - b) { return a - b; };
            template<typename T1, typename T2>
            auto sub(const T1& c1, const T2& c2){
                using ty1 = typename T1::value_type;
                using ty2 = typename T2::value_type;
                using bop = decltype(minus<ty1, ty2>);
                return bop_range<T1, T2, bop>(minus<ty1, ty2>, c1, c2);
            };

            template<typename T1, typename T2>
            constexpr auto multiply = [](T1 a, T2 b) -> decltype(a * b) { return a * b; };
            template<typename T1, typename T2>
            auto mul(const T1& c1, const T2& c2){
                using ty1 = typename T1::value_type;
                using ty2 = typename T2::value_type;
                using bop = decltype(multiply<ty1, ty2>);
                return bop_range<T1, T2, bop>(multiply<ty1, ty2>, c1, c2);
            };
        };
    };
}; 

#endif