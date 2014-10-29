#pragma once
#include <iterator>
#include <functional>
#include <string>
#include <iostream>
#include <vector>
#include <experimental/optional>


//#define METHODWRITE

class MethodWriter
{
public:
    MethodWriter(const std::string& name)
    : _name(name)
    {
#ifdef METHODWRITE
        std::cout << "[" << _depth++ << "] \t" << _name << " enter" << std::endl;
#endif
    }

    virtual ~MethodWriter() {
#ifdef METHODWRITE
        std::cout << "[" << --_depth << "] \t" << _name << " exit" << std::endl;
#endif
    }

private:
    std::string _name;
    static int _depth;
};

template<class QueryBuilder, class Query>
struct type_constructor
{
private:
    static Query get_query();
    static QueryBuilder get_builder();
public:
    typedef decltype(get_builder().build(get_query())) return_type;
};

template<typename Function, typename Arg>
struct function_traits
{
    typedef decltype(std::declval<Function>()(std::declval<Arg>())) return_type;
};

template <typename InputIterator, typename Function, typename OutputContainer, class A = std::allocator<typename OutputContainer::value_type> >
class bind_query
{
public:
    typedef A allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;

    typedef bind_query<InputIterator, Function, OutputContainer, A> this_type;

    class iterator
    {
    public:
        typedef typename A::value_type value_type;
        typedef typename A::difference_type difference_type;
        typedef typename A::reference reference;
        typedef typename A::pointer pointer;
        typedef std::input_iterator_tag iterator_category;

        typedef InputIterator input_iterator;
        typedef typename OutputContainer::iterator output_iterator;


        iterator()
                : _outerIt()
                , _outerEnd()
                , _innerIt()
                , _innerContainer()
                , _func(nullptr)
                , _name("default")
        {
            auto f(log("()"));
        }

        iterator(const std::string& name, const input_iterator& it, const input_iterator& end, Function * func)
                : _outerIt(it)
                , _outerEnd(end)
                , _innerIt()
                , _innerContainer()
                , _func(func)
                , _name(name)
        {
            auto f(log("(full)"));
            while(_outerIt != _outerEnd)
            {
                _innerContainer = (*_func)(*_outerIt);
                _innerIt = _innerContainer.begin();
                if(_innerIt != _innerContainer.end())
                    return;
                ++_outerIt;
            }
        }

        iterator(const iterator& other)
                : _outerIt(other._outerIt)
                , _outerEnd(other._outerEnd)
                , _innerIt(other._innerIt)
                , _innerContainer(other._innerContainer)
                , _func(other._func)
                , _name(other._name)
        {
            auto f(log("(copy)"));
        }

        ~iterator()
        {
            auto f(log("(destructor)"));
        }

        iterator& operator=(iterator other)
        {
            auto f(log("operator="));
            std::swap(*this, other);
        }

        bool operator==(const iterator& other) const
        {
            auto f(log("operator=="));
            if(_outerIt != other._outerIt)
                return false;
            if(_outerIt != _outerEnd)
                return (_innerIt == other._innerIt );
            else
                return true;
        }

        bool operator!=(const iterator& other) const
        {
            auto f(log("operator!="));
            if(_outerIt != other._outerIt)
                return true;
            if(_outerIt != _outerEnd)
                return (_innerIt != other._innerIt);
            else
                return false;
        }

        iterator& operator++()
        {
            auto f(log("operator++"));
            if(++_innerIt == _innerContainer.end())
            {
                while(++_outerIt != _outerEnd)
                {
                    _innerContainer = (*_func)(*_outerIt);
                    _innerIt = _innerContainer.begin();
                    if(_innerIt != _innerContainer.end())
                        break;
                }
            }
            return *this;
        }

        value_type operator*() const
        {
            auto f(log("operator*"));
            return *_innerIt;
        }

        pointer operator->() const
        {
            auto f(log("operator->"));
            return _innerIt.operator->();
        }

    private:
        MethodWriter log(const std::string& name) const
        {
            return MethodWriter(_name + "::" + name);
        }

        input_iterator _outerIt;
        input_iterator _outerEnd;
        output_iterator _innerIt;
        OutputContainer _innerContainer;
        Function* _func;

        std::string _name;
    };

    bind_query(const std::string& name,
            const InputIterator& first,
            const InputIterator& last,
            Function func)
            : _first(first)
            , _last(last)
            , _func(func)
            , _name(name)
    {}

    bind_query(const bind_query& other)
    : _first(other._first)
    , _last(other._last)
    , _func(other._func)
    {
    }

    ~bind_query() {}

    bind_query& operator=(const bind_query&);
    bool operator==(const bind_query&) const;
    bool operator!=(const bind_query&) const;

    iterator begin()
    {
        auto f(log("begin"));
        return iterator(_name + "::iterator", _first, _last, &_func);
    }

    iterator begin() const
    {
        auto f(log("begin const"));
        return iterator(_name + "::iterator", _first, _last, &_func);
    }

    iterator cbegin() const
    {
        auto f(log("cbegin"));
        return iterator(_name + "::iterator", _first, _last, &_func);
    }

    iterator end()
    {
        auto f(log("end"));
        return iterator(_name + "::iterator", _last, _last, &_func);
    }
    iterator end() const
    {
        auto f(log("end const"));
        return iterator(_name + "::iterator", _last, _last, &_func);
    }

    iterator cend() const
    {
        auto f(log("cend"));
        return iterator(_name + "::iterator", _last, _last, &_func);
    }

    void swap(bind_query& other)
    {
        std::swap(_first, other._first);
        std::swap(_last, other._last);
    }

    bool empty()
    {
        return _first != _last;
    }

    template<typename QueryBuilder>
    type_constructor<QueryBuilder, this_type> operator>>(QueryBuilder qb)
    {
        return qb.build(*this);
    }


private:
    MethodWriter log(const std::string& function)
    {
        return MethodWriter(_name + "::" + function);
    }


    InputIterator _first;
    InputIterator _last;
    Function _func;
    std::string _name;
};

template <typename InputIterator, typename Function>
bind_query<InputIterator, Function, decltype(std::declval<Function>()(std::declval<typename InputIterator::value_type>()))>
make_bind_query(
        const std::string& name,
        const InputIterator& first,
        const InputIterator& last,
        Function func)
{
    typedef decltype(std::declval<Function>()(std::declval<typename InputIterator::value_type>())) FunctionReturnType;
    return bind_query<InputIterator, Function, FunctionReturnType>(name, first, last, func);
}

template<class Function>
class bind_query_builder
{
public:

    bind_query_builder(Function func) : _func(func) {
    }

    template<typename Query>
    bind_query<typename Query::iterator, Function, function_traits<Function, typename Query::value_type>>
    build(Query q) const
    {
        return bind_query<typename Query::iterator, Function, function_traits<Function, typename Query::value_type>>
                ("bind", q.begin(), q.end(), _func);
    }
private:

    Function _func;
};

template <class Function>
bind_query_builder<Function> select_many(Function func)
{
    return bind_query_builder<Function>(func);
}