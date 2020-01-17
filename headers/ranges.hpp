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
            class iterator : 
                    public std::iterator<std::forward_iterator_tag, typename it::value_type>{
                public:
                    using value_type = typename it::value_type;
                    using reference = typename it::reference;
                    using const_reference = typename it::reference;
                protected:
                    it _current;
                public:
                    iterator(it current):
                        _current(current)
                        {};
                    iterator(iterator<it>& prev_it):
                        iterator(prev_it._current)
                        {};
                    iterator<it>& operator++(void){
                        (this->_current)++;
                        return *this;
                    };
                    iterator<it> operator++(int){
                        iterator ret_val(*this);
                        operator++();
                        return std::move(ret_val);
                    };
                    bool operator==(const iterator<it>& oth) const{
                        return (this->_current) == (oth._current);
                    };
                    bool operator!=(const iterator<it>& oth) const{
                        return !operator==(oth);
                    };
                    reference operator*(void){
                        return *(this->_current);
                    };
                    const_reference operator*(void) const{
                        return *(this->_current);
                    };
                    value_type operator*(int) const{
                        return *(this->_current);
                    };
                    it current(void) const{
                        return _current;
                    };
            };
            template<typename it>
            class num_iterator : public iterator<it>{
                public:
                    using value_type = typename it::value_type;
                    using reference = typename it::reference;
                    using const_reference = const typename it::reference;
                protected:
                    const it _begin;
                public:
                    num_iterator(const it begin, it current):
                        iterator<it>(current),
                        _begin(begin)
                        {};
                    num_iterator(const num_iterator<it>& prev_it):
                        num_iterator(prev_it._begin, prev_it._current)
                        {};
                    num_iterator<it>& operator++(void){
                        (this->_current) = std::next(this->_current);
                        return *this;
                    };
                    num_iterator<it> operator++(int){
                        num_iterator<it> ret_val(*this);
                        operator++();
                        return std::move(ret_val);
                    };
                    bool operator==(const num_iterator<it>& oth) const{
                        return iterator<it>::operator==(oth) && (_begin == oth._begin);
                    };
                    bool operator!=(const num_iterator<it>& oth) const{
                        return !operator==(oth);
                    };
                    std::size_t num(void) const{
                        return std::distance(_begin, this->_current);
                    };
            };
            template<typename it>
            class subs_iterator : public num_iterator<it>{
                public:
                    using value_type = typename it::value_type;
                    using reference = typename it::reference;
                    using const_reference = const typename it::reference;
                    using subs = typename std::pair<const std::size_t, value_type>;
                protected:
                    subs _body;
                public:
                    subs_iterator(const it begin, it current, subs body):
                        num_iterator<it>(begin, current),
                        _body(body)
                        {};
                    subs_iterator(const subs_iterator& prev_it):
                        subs_iterator(prev_it._begin, prev_it._end, prev_it._body)
                        {};
                    bool operator==(const subs_iterator<it>& oth){
                        if(_body != oth._body)
                            throw std::logic_error("Incomparable iterators");
                        return iterator<it>::operator==(oth);
                    };
                    bool operator!=(const subs_iterator<it>& oth){
                        return !operator==(oth);
                    };
                    value_type operator*(void) const{
                        const bool out = num_iterator<it>::num() == _body.first;
                        return out ? _body.second : num_iterator<it>::operator*();
                    };
            };
            template<typename it1, typename it2, typename op>
            class bop_iterator: public std::iterator<std::forward_iterator_tag, 
                    typename std::result_of<op(typename it1::value_type, typename it2::value_type)>::type >{
                public:
                    using ty1 = typename it1::value_type;
                    using ty2 = typename it2::value_type;
                    using value_type = typename std::result_of<op(ty1, ty2)>::type;
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
                    value_type operator*(void) const{
                        return _oper(*_cur1, *_cur2);
                    };
                    value_type operator++(void){
                        _cur1++;
                        _cur2++;
                        return operator*();
                    };
                    value_type operator++(int){
                        value_type ret_val = operator*();
                        operator++();
                        return std::move(ret_val);
                    };
                    bool operator==(const bop_iterator<it1, it2, op>& oth){
                        return (_cur1 == oth._cur1) && (_cur2 == oth._cur2);
                    };
                    bool operator!=(const bop_iterator<it1, it2, op>& oth){
                        return !operator==(oth);
                    };
                    it1 current(void) const{
                        return *_cur1;
                    };
            };
            template<typename T>
            class scalar_iterator: public std::iterator<std::forward_iterator_tag, T>{
                public:
                    using value_type = T;
                    using reference = const T&;
                    using const_reference = const T&;
                protected:
                    std::size_t _num;
                    const T _body;
                public:
                    scalar_iterator(const std::size_t num, const T& body):
                        _num(num),
                        _body(body)
                        {};
                    scalar_iterator(const scalar_iterator<T>& si):
                        scalar_iterator(si._num, si._body)
                        {};
                    value_type operator*(int) const{
                        return _body;
                    };
                    const_reference operator*(void) const{
                        return _body;
                    };
                    const_reference body(void) const{
                        return _body;
                    };
                    scalar_iterator<T>& operator++(void){
                        _num++;
                        return *this;
                    };
                    scalar_iterator<T> operator++(int){
                        scalar_iterator<T> ret_val(*this);
                        _num++;
                        return ret_val;
                    };
                    bool operator==(const scalar_iterator<T>& si) const{
                        if(_body != si._body)
                            throw std::logic_error("Incomparable iterators");
                        return (_num == si._num);
                    };
                    bool operator!=(const scalar_iterator<T>& si) const{
                        return operator==(si);
                    };
                    std::size_t current(void) const{
                        return _num;
                    };
            };
        };
        namespace dists{
            template<typename it>
            typename it::difference_type distance(it first, it last){
                //std::cout << "Std distance" << std::endl;
                return std::distance(first, last);
            };
            template<typename it>
            typename iters::iterator<it>::difference_type distance(
                    iters::iterator<it> first,  iters::iterator<it> last){
                //std::cout << "Iterator distance" << std::endl;
                return distance(first.current(), last.current());
            };
            template<typename it>
            typename iters::num_iterator<it>::difference_type distance(
                    iters::num_iterator<it> first,  iters::num_iterator<it> last){
                //std::cout << "Num iterator distance" << std::endl;
                return distance(first.current(), last.current());
            };
            template<typename it>
            typename iters::subs_iterator<it>::difference_type distance(
                    iters::subs_iterator<it> first,  iters::subs_iterator<it> last){
                //std::cout << "Subs iterator distance" << std::endl;
                return distance<it>(first.current(), last.current());
            };
            template<typename it1, typename it2, typename op>
            typename iters::bop_iterator<it1, it2, op>::difference_type distance(
                    iters::bop_iterator<it1, it2, op> first, iters::bop_iterator<it1, it2, op> last){
                //std::cout << "Bop iterator distance" << std::endl;
                return distance(first.current(), last.current());
            };
            template<typename T>
            typename iters::scalar_iterator<T>::difference_type distance(
                    iters::scalar_iterator<T> first,  iters::scalar_iterator<T> last){
                //std::cout << "Scalar iterator distance" << std::endl;
                return last.current() - first.current();
            };
        };
        template<typename T, typename it>
        class range_proto{
            public:
                using container = T;
                using iterator = typename iters::iterator<it>;
                using const_iterator = typename iters::iterator<it>;
                using reference = typename T::const_reference;
                using const_reference = typename T::const_reference;
                using value_type = typename T::value_type;
            public:
                const it _begin, _end;
            public:
                range_proto(const it begin, const it end):
                    _begin(begin), _end(end)
                    {};
                std::size_t size(void) const{
                    return std::distance(_begin, _end);
                };
            public:
                iterator begin(void) const{
                    return iterator(_begin);
                };
                iterator end(void) const{
                    return iterator(_end);
                };
                iterator iterator_at(const std::size_t num){
                    return iterator(std::next(_begin, num));
                };
                iterator iterator_at(const std::size_t num) const{
                    return iterator(std::next(_begin, num));
                };
                reference at(const size_t num){
                    return *std::next(_begin, num);
                };
                const_reference at(const size_t num) const{
                    return *std::next(_begin, num);
                };
        };
        template<typename T>
        class const_range : public range_proto<T, typename T::const_iterator>{
            private:
                using proto_iterator = typename T::const_iterator;
                using proto_range = range_proto<T, proto_iterator>;
            public:
                using container = T;
                using iterator = proto_iterator;
                using const_iterator = proto_iterator;
                using reference = typename T::const_reference;
                using const_reference = typename T::const_reference;
                using value_type = typename T::value_type;
            public:
                const_iterator cbegin(void) const{
                    return this->_begin;
                };
                const_iterator cend(void) const{
                    return this->_end;
                };
            public:
                const_range(const proto_iterator begin, const proto_iterator end):
                    range_proto<T, proto_iterator>(begin, end)
                    {};
                const_range(const const_range<T>& cont):
                    range_proto<T, proto_iterator>(cont._begin, cont._end)
                    {};
                const_range(const T& cont):
                    const_range(cont.cbegin(), cont.cend())
                    {};
        };
        template<typename T>
        class num_range : public const_range<T>{
            private:
                using proto_iterator = typename T::const_iterator;
                using proto_range = range_proto<T, proto_iterator>;
            public:
                using container = T;
                using iterator = typename iters::num_iterator<proto_iterator>;
                using const_iterator = typename iters::num_iterator<proto_iterator>;
                using reference = typename T::reference;
                using const_reference = const typename T::reference;
                using value_type = typename T::value_type;
            public:
                num_range(const proto_iterator beg, const proto_iterator end):
                    const_range<T>(beg, end)
                    {};
                num_range(const const_range<T>& cr):
                    num_range(cr.cbegin(), cr.cend())
                    {};
                num_range(const num_range<T>& cr):
                    num_range(cr.cbegin(), cr.cend())
                    {};
                num_range(const T& co):
                    num_range(co.cbegin(), co.cend())
                    {};
            public:
                const_iterator cbegin(void) const{
                    return const_iterator(this->_begin, this->_begin);
                };
                const_iterator cend(void) const{
                    return const_iterator(this->_begin, this->_end);
                };
                const_iterator iterator_at(const std::size_t num) const{
                    const auto current = const_range<T>::iterator_at(num).current();
                    return const_iterator(this->_begin, current);
                };
        };
        template<typename T> 
        class subs_range : public num_range<T>{
            private:
                using proto_iterator = typename T::const_iterator;
                using proto_range = range_proto<T, proto_iterator>;
            public:
                using container = T;
                using iterator = typename iters::subs_iterator<proto_iterator>;
                using const_iterator = typename iters::subs_iterator<proto_iterator>;
                using reference = typename T::reference;
                using const_reference = const typename T::reference;
                using value_type = typename T::value_type;
                using subs = typename std::pair<const std::size_t, value_type>;
            protected:
                const subs _body;
            public:
                subs_range(const proto_iterator beg, proto_iterator cur, const subs body):
                    num_range<T>(beg, cur),
                    _body(std::move(body))
                    {};
                subs_range(const T& cont, const subs body):
                    subs_range(cont.cbegin(), cont.cend(), body)
                    {};
                template<typename P>
                subs_range(const P& cont, const subs body):
                    subs_range(cont.cbegin(), cont.cend(), body)
                    {};
            public:
                const_iterator begin(void) const{
                    return const_iterator(this->_begin, this->_begin, _body);
                };
                const_iterator end(void) const{
                    return const_iterator(this->_begin, this->_end, _body);
                };
                const_iterator cbegin(void) const{
                    return begin();
                };
                const_iterator cend(void) const{
                    return end();
                };
                const_iterator iterator_at(const std::size_t num) const{
                    const auto current = num_range<T>::iterator_at(num).current();
                    return const_iterator(this->_begin, current, _body);
                };
                /*const_reference at(const std::size_t num) const{
                    return (num == _body.first) ? _body.second : num_range<T>::at(num);
                };*/
                value_type at(const std::size_t num) const{
                    return (num == _body.first)? _body.second :num_range<T>::at(num);
                };
        }; 
        template<typename T>
        class scalar_range{
            public:
                using value_type = T;
                using reference = const T&;
                using const_reference = const T&;
                using iterator = iters::scalar_iterator<T>;
                using const_iterator = iters::scalar_iterator<T>;
            protected:
                const std::size_t _size;
                const T _body;
                void check(const std::size_t num) const{
                    if(num > _size) throw std::length_error("Num should be less than scalar_range size.");
                };
            public:
                scalar_range(const std::size_t size, const T& body):
                    _size(size), _body(body)
                    {};
                scalar_range(const scalar_range<T>& sr):
                    _size(sr._size), _body(sr._body)
                    {};
                std::size_t size(void) const{
                    return _size;
                };
                const T& body(void) const{
                    return _body;
                };
                const_iterator cbegin(void) const{
                    return iterator(0, _body);
                };
                const_iterator cend(void) const{
                    return iterator(_size, _body);
                };
                iterator begin(void) const{
                    return cbegin();
                };
                iterator end(void) const{
                    return cend();
                };
                iterator iterator_at(const std::size_t num){
                    check(num);
                    return iterator(num, _body);
                };
                value_type at(const std::size_t num){
                    check(num);
                    return _body;
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
                    _size(dists::distance(_beg1, _end1))
                    {
                        if(c1.size() != c2.size())
                            throw std::length_error("cont1 should have same length with cont2");
                    };
                bop_range(const op& oper, const T1& c1, const T2& c2):
                    _oper(oper),
                    _beg1(c1.cbegin()), _end1(c1.cend()),
                    _beg2(c2.cbegin()), _end2(c2.cend()),
                    _size(dists::distance(_beg1, _end1))
                    {
                        if(c1.size() != c2.size())
                            throw std::length_error("cont1 should have same length with cont2");
                    };
                iterator cbegin(void) const{
                    return iterator(_oper, _beg1, _beg2);
                };
                iterator cend(void) const{
                    return iterator(_oper, _end1, _end2);
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
    };
}; 

#endif