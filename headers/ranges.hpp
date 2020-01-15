#ifndef RANGES
#define RANGES

#include <utility>
#include <iterator>
#include <algorithm>
#include <functional>

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
                    const it _begin, _current;
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
                        return _current;
                    };
                    iterator<it> operator++(int){
                        iterator ret_val(*this);
                        operator++();
                        return ret_val;
                    };
                    bool operator==(const iterator<it>& oth){
                        return 
                            (_current == oth._current) 
                                && 
                            (_begin == oth._begin);
                    };
                    bool operator!=(const iterator<it>& oth){
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
    };
}; 

#endif