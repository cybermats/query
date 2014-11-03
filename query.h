#pragma once
#include <iterator>
#include <cassert>
#include <vector>
#include <algorithm>

#define CMAKE_TYPENAME

namespace query {

	/*************************************************************//**
	 * get_builder
	 ****************************************************************/
    template<typename QueryBuilder, typename Query>
    struct get_builtup_type {
        static QueryBuilder get_builder();

        static Query get_query();

        typedef decltype(get_builder().build(get_query())) type;
    };

	/*************************************************************//**
	 * function_traits
	 ****************************************************************/
    template<typename Function, typename Arg>
    struct function_traits
    {
        static Function get_function();
        static Arg get_argument();
        typedef decltype(get_function()(get_argument())) return_type;
    };

	/*************************************************************//**
	 * base_query
	 ****************************************************************/
    class base_query
    {
    protected:
        base_query() {}
        base_query(const base_query &) {}
        base_query(base_query&&) {}
        ~base_query() {}
    };

	/*************************************************************//**
	 * base_query_builder
	 ****************************************************************/
    class base_query_builder
    {
    protected:
        base_query_builder() {}
        base_query_builder(const base_query_builder &) {}
        base_query_builder(base_query_builder&&) {}
        ~base_query_builder() {}
    };


	/*************************************************************//**
	 * simple_query
	 ****************************************************************/
    template<typename InputIterator, class A = std::allocator<typename InputIterator::value_type> >
    class simple_query {
    public:
        typedef A allocator_type;
        typedef typename A::value_type value_type;
        typedef typename A::reference reference;
        typedef typename A::const_reference const_reference;
        typedef typename A::difference_type difference_type;
        typedef typename A::size_type size_type;

        typedef simple_query<InputIterator, A> this_type;

        class iterator {
        public:
            typedef typename A::value_type value_type;
            typedef typename A::difference_type difference_type;
            typedef typename A::reference reference;
            typedef typename A::pointer pointer;
            typedef std::input_iterator_tag iterator_category;

            iterator(InputIterator current)
                    : _current(current) {
            }

            iterator(const iterator &other)
                    : _current(other._current) {
            }

            bool operator==(const iterator &other) const {
                return _current == other._current;
            }

            bool operator!=(const iterator &other) const {
                return _current != other._current;
            }

            iterator &operator++() {
                ++_current;
                return *this;
            }

            value_type operator*() const {
                return *_current;
            }

            pointer operator->() const {
                return _current.operator->();
            }

        private:
            InputIterator _current;
        };

        simple_query(
                const InputIterator &first,
                const InputIterator &last)
                : _first(first), _last(last) {
        }

        simple_query(const simple_query &other)
                : _first(other._first), _last(other._last) {
        }

        ~simple_query() {
        }

        simple_query &operator=(const simple_query &);

        bool operator==(const simple_query &) const;

        bool operator!=(const simple_query &) const;

        iterator begin() const {
            return iterator(_first);
        }

        iterator end() const {
            return iterator(_last);
        }

        void swap(simple_query &other) {
            std::swap(_first, other._first);
            std::swap(_last, other._last);
        }

        bool empty() {
            return _first != _last;
        }

        template<typename QueryBuilder>
        typename get_builtup_type<QueryBuilder, this_type>::type operator>>(QueryBuilder qb) const {
            return qb.build(*this);
        }

    private:
        InputIterator _first;
        InputIterator _last;
    };

	/*************************************************************//**
	 * simple_query_builder
	 ****************************************************************/
    class simple_query_builder {
    public:

        template<typename Query>
        simple_query<typename Query::iterator> build(Query query) const {
            return simple_query<typename Query::iterator>(query.begin(), query.end());

        }
    };


	/*************************************************************//**
	 * int_query
	 ****************************************************************/
    class int_query {
    public:
        typedef std::allocator<int> A;
        typedef A allocator_type;
		typedef CMAKE_TYPENAME A::value_type value_type;
		typedef CMAKE_TYPENAME A::reference reference;
		typedef CMAKE_TYPENAME A::const_reference const_reference;
		typedef CMAKE_TYPENAME A::difference_type difference_type;
		typedef CMAKE_TYPENAME A::size_type size_type;

        typedef int_query this_type;

        class iterator {
        public:
			typedef CMAKE_TYPENAME A::value_type value_type;
			typedef CMAKE_TYPENAME A::difference_type difference_type;
			typedef CMAKE_TYPENAME A::reference reference;
			typedef CMAKE_TYPENAME A::pointer pointer;
            typedef std::input_iterator_tag iterator_category;

            iterator(int current)
                    : _current(current) {
            }

            iterator(const iterator &other)
                    : _current(other._current) {
            }

            bool operator==(const iterator &other) const {
                return _current == other._current;
            }

            bool operator!=(const iterator &other) const {
                return _current != other._current;
            }

            iterator &operator++() {
                ++_current;
                return *this;
            }

            value_type operator*() const {
                return _current;
            }

            pointer operator->() const {
                assert(false);
            }

        private:
            int _current;
        };

        int_query(int begin, int end)
                : _begin(begin)
                , _end(end)
        {
        }

        int_query(const int_query &other)
                : _begin(other._begin)
                , _end(other._end) {
        }

        ~int_query() {
        }

        int_query &operator=(const int_query &);

        bool operator==(const int_query &) const;

        bool operator!=(const int_query &) const;

        iterator begin() const {
            return iterator(_begin);
        }

        iterator end() const {
            return iterator(_end);
        }

        void swap(int_query &other) {
            std::swap(_begin, other._begin);
            std::swap(_end, other._end);
        }

        bool empty() {
            return _begin == _end;
        }

        template<typename QueryBuilder>
        typename get_builtup_type<QueryBuilder, this_type>::type operator>>(QueryBuilder qb) const {
            return qb.build(*this);
        }

    private:
        int _begin;
        int _end;
    };


	/*************************************************************//**
	 * where_query
	 ****************************************************************/
    template<typename InputIterator, typename Predicate, class A = std::allocator<typename InputIterator::value_type> >
    class where_query {
    public:
        typedef A allocator_type;
        typedef typename A::value_type value_type;
        typedef typename A::reference reference;
        typedef typename A::const_reference const_reference;
        typedef typename A::difference_type difference_type;
        typedef typename A::size_type size_type;

        typedef where_query<InputIterator, Predicate, A> this_type;

        class iterator {
        public:
            typedef typename A::value_type value_type;
            typedef typename A::difference_type difference_type;
            typedef typename A::reference reference;
            typedef typename A::pointer pointer;
            typedef std::input_iterator_tag iterator_category;

            iterator(InputIterator current,
                    InputIterator last,
                    Predicate pred)
                    : _current(current)
                    , _last(last)
                    , _pred(pred)
            {
                if(_pred(*_current))
                    return;

                while(++_current != _last)
                {
                    if(_pred(*_current))
                        break;
                }
            }

            iterator(const iterator &other)
                    : _current(other._current)
                    , _last(other._last)
                    , _pred(other._pred)
            {
            }

            bool operator==(const iterator &other) const {
                return _current == other._current;
            }

            bool operator!=(const iterator &other) const {
                return _current != other._current;
            }

            iterator &operator++() {
                assert(_current != _last);
                while(++_current != _last)
                {
                    if(_pred(*_current))
                        break;
                }
                return *this;
            }

            value_type operator*() const {
                assert(_current != _last);
                return *_current;
            }

            pointer operator->() const {
                assert(_current != _last);
                return _current.operator->();
            }

        private:
            InputIterator _current;
            InputIterator _last;
            Predicate _pred;
        };

        where_query(
                const InputIterator &first,
                const InputIterator &last,
                const Predicate &pred)
                : _first(first), _last(last), _pred(pred) {
        }

        where_query(const where_query &other)
                : _first(other._first), _last(other._last), _pred(other._pred) {
        }

        ~where_query() {
        }

        where_query &operator=(const where_query &);

        bool operator==(const where_query &) const;

        bool operator!=(const where_query &) const;

        iterator begin() const {
            return iterator(_first, _last, _pred);
        }

        iterator end() const {
            return iterator(_last, _last, _pred);
        }

        void swap(where_query &other) {
            std::swap(_first, other._first);
            std::swap(_last, other._last);
            std::swap(_pred, other._pred);
        }

        bool empty() {
            return _first != _last;
        }

        template<typename QueryBuilder>
        typename get_builtup_type<QueryBuilder, this_type>::type operator>>(QueryBuilder qb) const {
            return qb.build(*this);
        }

    private:
        InputIterator _first;
        InputIterator _last;
        Predicate _pred;
    };

	/*************************************************************//**
	 * where_query_builder
	 ****************************************************************/
	template<typename Predicate>
    class where_query_builder {
    public:
        where_query_builder(Predicate pred) : _pred(pred) {
        }

        template<typename Query>
        where_query<typename Query::iterator, Predicate> build(Query query) const {
            return where_query<typename Query::iterator, Predicate>(query.begin(), query.end(), _pred);

        }
    private:
        Predicate _pred;

    };

	/*************************************************************//**
	 * select_query
	 ****************************************************************/
    template<typename InputIterator, typename Generator>
    class select_query {
    public:
        static typename InputIterator::value_type get_source();
        static Generator get_generator();

        typedef decltype(get_generator()(get_source())) raw_value_type;
        typedef std::allocator<raw_value_type> A;


        typedef A allocator_type;
        typedef typename A::value_type value_type;
        typedef typename A::reference reference;
        typedef typename A::const_reference const_reference;
        typedef typename A::difference_type difference_type;
        typedef typename A::size_type size_type;

        typedef select_query<InputIterator, Generator> this_type;

        class iterator {
        public:
            typedef typename A::value_type value_type;
            typedef typename A::difference_type difference_type;
            typedef typename A::reference reference;
            typedef typename A::pointer pointer;
            typedef std::input_iterator_tag iterator_category;

            iterator(InputIterator current,
                    InputIterator last,
                    Generator generator)
                    : _current(current)
                    , _last(last)
                    , _generator(generator)
            {
            }

            iterator(const iterator &other)
                    : _current(other._current)
                    , _last(other._last)
                    , _generator(other._generator)
            {
            }

            bool operator==(const iterator &other) const {
                return _current == other._current;
            }

            bool operator!=(const iterator &other) const {
                return _current != other._current;
            }

            iterator &operator++() {
                assert(_current != _last);
                ++_current;
                return *this;
            }

            value_type operator*() const {
                assert(_current != _last);
                return _generator(*_current);
            }

            pointer operator->() const {
                assert(_current != _last);
                return _current.operator->();
            }

        private:
            InputIterator _current;
            InputIterator _last;
            Generator _generator;
        };

        select_query(
                const InputIterator &first,
                const InputIterator &last,
                const Generator &generator)
                : _first(first), _last(last), _generator(generator) {
        }

        select_query(const select_query &other)
                : _first(other._first), _last(other._last), _generator(other._generator) {
        }

        ~select_query() {
        }

        select_query &operator=(const select_query &);

        bool operator==(const select_query &) const;

        bool operator!=(const select_query &) const;

        iterator begin() const {
            return iterator(_first, _last, _generator);
        }

        iterator end() const {
            return iterator(_last, _last, _generator);
        }

        void swap(select_query &other) {
            std::swap(_first, other._first);
            std::swap(_last, other._last);
            std::swap(_generator, other._generator);
        }

        bool empty() {
            return _first != _last;
        }

        template<typename QueryBuilder>
        typename get_builtup_type<QueryBuilder, this_type>::type operator>>(QueryBuilder qb) const {
            return qb.build(*this);
        }

    private:
        InputIterator _first;
        InputIterator _last;
        Generator _generator;
    };

	/*************************************************************//**
	 * select_query_builder
	 ****************************************************************/
    template<typename Generator>
    class select_query_builder {
    public:

        typedef char output_type;

        select_query_builder(Generator generator) : _generator(generator) {
        }

        template<typename Query>
        select_query<typename Query::iterator, Generator>
        build(Query query) const {
			return select_query<
                    typename Query::iterator, Generator>(query.begin(), query.end(), _generator);
        }
    private:
        Generator _generator;

    };

	/*************************************************************//**
	 * sorting_query_builder
	 ****************************************************************/
    class sorting_query_builder
    {
    protected:
        sorting_query_builder() {}
        sorting_query_builder(const sorting_query_builder &) {}
        sorting_query_builder(sorting_query_builder&&) {}
        ~sorting_query_builder() {}
    };


	/*************************************************************//**
	 * orderby_query
	 ****************************************************************/
    template<typename InputIterator, typename Predicate, class A = std::allocator<typename InputIterator::value_type> >
    class orderby_query {
    public:
        typedef A allocator_type;
        typedef typename A::value_type value_type;
        typedef typename A::reference reference;
        typedef typename A::const_reference const_reference;
        typedef typename A::difference_type difference_type;
        typedef typename A::size_type size_type;

        typedef orderby_query<InputIterator, Predicate, A> this_type;
        typedef typename std::vector<value_type>::iterator output_iterator;


        class iterator {
        public:
            typedef typename A::value_type value_type;
            typedef typename A::difference_type difference_type;
            typedef typename A::reference reference;
            typedef typename A::pointer pointer;
            typedef std::input_iterator_tag iterator_category;

            iterator(output_iterator current)
                    : _current(current)
            {
            }

            iterator(const iterator &other)
                    : _current(other._current)
            {
            }

            bool operator==(const iterator &other) const {
                return _current == other._current;
            }

            bool operator!=(const iterator &other) const {
                return _current != other._current;
            }

            iterator &operator++() {
                ++_current;
                return *this;
            }

            value_type operator*() const {
                return *_current;
            }

            pointer operator->() const {
                return _current.operator->();
            }

        private:
            output_iterator _current;
        };

        orderby_query(
                const InputIterator &first,
                const InputIterator &last,
                const Predicate &pred,
                bool sort_ascending)
                : _first(first)
                , _last(last)
                , _pred(pred)
                , _sort_ascending(sort_ascending)
                , _sorted_values()
                , _initialized(false)
        {
        }

        orderby_query(const orderby_query &other)
                : _first(other._first)
                , _last(other._last)
                , _pred(other._pred)
                , _sort_ascending(other._sort_ascending)
                , _sorted_values(other._sorted_values)
                , _initialized(other._initialized)
        {
        }

        ~orderby_query()
        {
        }

        orderby_query &operator=(const orderby_query &);

        bool operator==(const orderby_query &) const;

        bool operator!=(const orderby_query &) const;

        iterator begin() const {
            if(!_initialized)
                initialize();
            return iterator(_sorted_values.begin());
        }

        iterator end() const {
            if(!_initialized)
                initialize();
            return iterator(_sorted_values.end());
        }

        void swap(orderby_query &other) {
            std::swap(_first, other._first);
            std::swap(_last, other._last);
            std::swap(_pred, other._pred);
            std::swap(_sort_ascending, other._sort_ascending);
            std::swap(_sorted_values, other._sorted_values);
            std::swap(_initialized, other._initialized);
        }

        bool empty() {
            return _first != _last;
        }

        template<typename QueryBuilder>
        typename get_builtup_type<QueryBuilder, this_type>::type operator>>(QueryBuilder qb) const {
            return qb.build(*this);
        }

    private:
        bool compare_ascending(const value_type & lhs, const value_type & rhs) const
        {
            return _pred(lhs, rhs);
        }

        bool compare_descending(const value_type & lhs, const value_type & rhs) const
        {
            return _pred(rhs, lhs);
        }

        void initialize() const
        {
            _initialized = true;
            _sorted_values.clear();
            if(_first == _last)
                return;

            _sorted_values.reserve(16U);
            _sorted_values.insert(_sorted_values.end(), _first, _last);
            if(_sort_ascending) {
                std::sort(
                        _sorted_values.begin(),
                        _sorted_values.end(),
                        [this](const value_type &lhs, const value_type &rhs) {
                            return this->compare_ascending(lhs, rhs);
                        });
            } else {
                std::sort(
                        _sorted_values.begin(),
                        _sorted_values.end(),
                        [this](const value_type &lhs, const value_type &rhs) {
                            return this->compare_descending(lhs, rhs);
                        });
            }
        }

        InputIterator _first;
        InputIterator _last;
        Predicate _pred;
        bool _sort_ascending;
        mutable std::vector<value_type> _sorted_values;
        mutable bool _initialized;
    };

	/*************************************************************//**
	 * orderby_query_builder
	 ****************************************************************/
    template<typename Predicate>
    class orderby_query_builder : public sorting_query_builder {
    public:
        orderby_query_builder(Predicate pred, bool sort_ascending)
                : _pred(pred)
                , _sort_ascending(sort_ascending)
        {
        }

        template<typename Query>
        orderby_query<typename Query::iterator, Predicate> build(Query query) const {
            return orderby_query<typename Query::iterator, Predicate>(query.begin(), query.end(), _pred, _sort_ascending);

        }
    private:
        Predicate _pred;
        bool _sort_ascending;
    };


	/*************************************************************//**
	 * zip_with_query
	 ****************************************************************/
    template<typename InputIterator, typename OtherIterator,
            class A = std::allocator<std::pair<typename InputIterator::value_type, typename OtherIterator::value_type > > >
    class zip_with_query {
    public:
        typedef A allocator_type;
        typedef typename A::value_type value_type;
        typedef typename A::reference reference;
        typedef typename A::const_reference const_reference;
        typedef typename A::difference_type difference_type;
        typedef typename A::size_type size_type;

        typedef zip_with_query<InputIterator, OtherIterator, A> this_type;

        class iterator {
        public:
            typedef typename A::value_type value_type;
            typedef typename A::difference_type difference_type;
            typedef typename A::reference reference;
            typedef typename A::pointer pointer;
            typedef std::input_iterator_tag iterator_category;

            iterator(InputIterator current,
                    OtherIterator other_current)
                    : _current(current)
                    , _other_current(other_current)
            {
            }

            iterator(const iterator &other)
                    : _current(other._current)
                    , _other_current(other._other_current)
            {
            }

            bool operator==(const iterator &other) const {
                return _current == other._current ||
                       _other_current == other._other_current;
            }

            bool operator!=(const iterator &other) const {
                return _current != other._current &&
                       _other_current != other._other_current;
            }

            iterator &operator++() {
                ++_current;
                ++_other_current;
                return *this;
            }

            value_type operator*() const {
                return std::make_pair(*_current, *_other_current);
            }

        private:
            InputIterator _current;
            OtherIterator _other_current;
        };

        zip_with_query(
                const InputIterator &first,
                const InputIterator &last,
                const OtherIterator &other_first,
                const OtherIterator &other_last)
                : _first(first)
                , _last(last)
                , _other_first(other_first)
                , _other_last(other_last)
        {
        }

        zip_with_query(const zip_with_query &other)
                : _first(other._first)
                , _last(other._last)
                , _other_first(other._other_first)
                , _other_last(other._other_last)
        {
        }

        ~zip_with_query() {
        }

        zip_with_query &operator=(const zip_with_query &);

        bool operator==(const zip_with_query &) const;

        bool operator!=(const zip_with_query &) const;

        iterator begin() const {
            return iterator(_first, _other_first);
        }

        iterator end() const {
            return iterator(_last, _other_last);
        }

        void swap(zip_with_query &other) {
            std::swap(_first, other._first);
            std::swap(_last, other._last);
            std::swap(_other_first, other._other_first);
            std::swap(_other_last, other._other_last);
        }

        bool empty() {
            return _first == _last ||
                   _other_first == _other_last;
        }

        template<typename QueryBuilder>
        typename get_builtup_type<QueryBuilder, this_type>::type operator>>(QueryBuilder qb) const {
            return qb.build(*this);
        }

    private:
        InputIterator _first;
        InputIterator _last;
        OtherIterator _other_first;
        OtherIterator _other_last;
    };

	/*************************************************************//**
	 * zip_with_query_builder
	 ****************************************************************/
    template<typename OtherIterator>
    class zip_with_query_builder {
    public:

        zip_with_query_builder(OtherIterator other_first, OtherIterator other_last)
                : _other_first(other_first), _other_last(other_last) {
        }

        template<typename Query>
        zip_with_query<typename Query::iterator, OtherIterator> build(Query query) const {
            return zip_with_query<typename Query::iterator, OtherIterator>(
                    query.begin(),
                    query.end(),
                    _other_first,
                    _other_last);
        }

    private:
        OtherIterator _other_first;
        OtherIterator _other_last;
    };


}


/*************************************************************//**
 * lift
 ****************************************************************/
template<typename InputIterator>
query::simple_query<InputIterator>
lift(InputIterator first, InputIterator last)
{
    return query::simple_query<InputIterator>(first, last);
}

/*************************************************************//**
 * from_range
 ****************************************************************/
query::int_query from_range(int begin, int end)
{
    return query::int_query(begin, end);
}

/*************************************************************//**
 * from_range_infinite
 ****************************************************************/
query::int_query from_range_infinite(int begin)
{
    return query::int_query(begin, begin - 1);
}

/*************************************************************//**
 * pass_through
 ****************************************************************/
query::simple_query_builder pass_through()
{
    return query::simple_query_builder();
}

/*************************************************************//**
 * where
 ****************************************************************/
template<typename Predicate>
query::where_query_builder<Predicate>
where(Predicate pred)
{
    return query::where_query_builder<Predicate>(pred);
}

/*************************************************************//**
 * select
 ****************************************************************/
template<typename Generator>
query::select_query_builder<Generator>
select(Generator generator)
{
    return query::select_query_builder<Generator>(generator);
}

/*************************************************************//**
 * orderby
 ****************************************************************/
template<typename Predicate>
query::orderby_query_builder<Predicate>
orderby(Predicate pred, bool sort_ascending = true)
{
    return query::orderby_query_builder<Predicate>(pred, sort_ascending);
}

/*************************************************************//**
 * zip_with
 ****************************************************************/
template<typename OtherQuery>
query::zip_with_query_builder<typename OtherQuery::iterator>
zip_with(OtherQuery other_query)
{
    return query::zip_with_query_builder<typename OtherQuery::iterator>(other_query.begin(), other_query.end());
}

/*************************************************************//**
 * zip_with
 ****************************************************************/
template<typename OtherIterator>
query::zip_with_query_builder<OtherIterator>
zip_with(OtherIterator first, OtherIterator last)
{
    return query::zip_with_query_builder<OtherIterator>(first, last);
}