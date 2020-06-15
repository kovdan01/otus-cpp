#ifndef MATRIX_H
#define MATRIX_H

#include <memory>
#include <cstddef>
#include <unordered_map>
#include <array>
#include <utility>
#include <type_traits>
#include <optional>
#include <functional>

namespace my
{

// ***************************************************************************
//                     PRIMARY MATRIX CLASS DECLARATION
// ***************************************************************************


/**
 * @brief The Matrix class
 * Represents an infinite matrix with the given number of dimensions
 * and a default value for not explicitly defined cells
 * @tparam T Element type
 * @tparam dimensions Dimensions number
 * @tparam Allocator Custom allocator (optional)
 */
template <typename T,
          std::size_t dimensions,
          typename Allocator = std::allocator<T>>
class Matrix
{
public:
    friend Matrix<T, dimensions + 1, Allocator>;

    // *** Member types ***
    // ~~~~~~~~~~~~~~~~~~~~

    using ValueType     = T;
    using MatrixType    = Matrix<T, dimensions, Allocator>;
    using SubMatrixType = Matrix<T, dimensions - 1, Allocator>;
    using IndexType     = std::array<std::size_t, dimensions>;
    using SubIndexType  = std::array<std::size_t, dimensions - 1>;

private:
    using KeyValueType  = std::pair<const std::size_t, SubMatrixType>;
    using StorageType   = std::unordered_map<std::size_t,
                                             SubMatrixType,
                                             std::hash<std::size_t>,
                                             std::equal_to<std::size_t>,
                                             typename std::allocator_traits<Allocator>::template rebind_alloc<KeyValueType>
                                            >;

public:
    // *** Constructors ***
    // ~~~~~~~~~~~~~~~~~~~~

    /**
     * @brief Constructs an infinite matrix with default value
     * @param default_value Default value for matrix cells that are not explicitly defined
     */
    explicit Matrix(T default_value);

    /**
     * @brief Constructs an infinite matrix with default value equal to T{}
     */
    Matrix() = default;

    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;
    Matrix(Matrix&&) noexcept = default;
    Matrix& operator=(Matrix&&) noexcept = default;

    // *** Observers ***
    // ~~~~~~~~~~~~~~~~~

    /**
     * @brief Gets number of non-default elements
     * @return Number of non-default elements
     */
    [[nodiscard]] std::size_t size() const;

    /**
     * @brief Checks if matrix has non-default elements
     * @return `false` if matrix has non-default elements, `true` otherwise
     */
    [[nodiscard]] bool empty() const;

    // *** Mutators ***
    // ~~~~~~~~~~~~~~~~

    /**
     * @brief Removes empty submatrixes
     * While using an object of the class, it can accumulate empty submatrixes,
     * that increase memory usage. For example, assuming `m` is a 2-d matrix with default value 0:
     * `m[0][0] = 5; m[0][0] = 0;`. After that `m[0][0]` will be stored in memory, even
     * though it will not affect `size()` and `empty` output.
     */
    void shrink_to_fit();

    // *** Index access ***
    // ~~~~~~~~~~~~~~~~~~~~

    /**
     * @brief Gets submatrix at the given index
     * @param index Index of submatrix to get
     * @return The submatrix with `dimensions - 1` dimensions on index `index`
     */
    SubMatrixType& operator[](std::size_t index);
    const SubMatrixType& operator[](std::size_t index) const;

    /**
     * @brief Gets value at the given position
     * @param index Position in matrix
     * @return The matrix cell value on the given position
     */
    T& at(const IndexType& index);
    const T& at(const IndexType& index) const;

private:
    template <typename IndexIterator>
    T& at(IndexIterator begin, IndexIterator end);
    template <typename IndexIterator>
    const T& at(IndexIterator begin, IndexIterator end) const;

public:
    // *** Iterators ***
    // ~~~~~~~~~~~~~~~~~

    class Iterator;
    class ConstIterator;

    /**
     * @brief Gets the iterator to the begin of explicitly-defined elements range
     * @return Iterator to the begin of explicitly-defined elements range
     */
    Iterator begin();

    /**
     * @brief Gets the iterator to the end of explicitly-defined elements range
     * @return Iterator to the end of explicitly-defined elements range
     */
    Iterator end();

    /**
     * @brief Gets the constant iterator to the begin of explicitly-defined elements range
     * @return Constant iterator to the begin of explicitly-defined elements range
     */
    ConstIterator begin() const;

    /**
     * @brief Gets the constant iterator to the end of explicitly-defined elements range
     * @return Constant terator to the end of explicitly-defined elements range
     */
    ConstIterator end() const;

    class Iterator
    {
    private:
        enum class Type { BEGIN, END, };
        friend MatrixType;
        Iterator(MatrixType* instance, Type type);

    public:
        // *** Observers ***
        // ~~~~~~~~~~~~~~~~~

        /**
         * @brief Gets index and value of the element that iterator refers to
         * @return Index and value of the element that iterator refers to
         */
        std::pair<IndexType, T&> operator*() const;

        /**
         * @brief Gets index of the element that iterator refers to
         * @return Index of the element that iterator refers to
         */
        IndexType index() const;

        /**
         * @brief Gets value of the element that iterator refers to
         * @return Value of the element that iterator refers to
         */
        T& value() const;

        // *** Mutators ***
        // ~~~~~~~~~~~~~~~~

        /**
         * @brief Goes to the next element
         * @return Iterator referring to the next element
         */
        Iterator& operator++();

        /**
         * @brief Goes to the next element
         * @return Iterator referring to the current element
         */
        const Iterator operator++(int);

        // *** Compare operators ***
        // ~~~~~~~~~~~~~~~~~~~~~~~~~

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        MatrixType* m_instance;
        std::optional<typename SubMatrixType::ConstIterator> m_sub_iterator;
        typename StorageType::const_iterator m_current_iterator;
    };

    class ConstIterator
    {
    private:
        enum class Type { BEGIN, END, };
        friend MatrixType;
        ConstIterator(const MatrixType* instance, Type type);

    public:
        // *** Observers ***
        // ~~~~~~~~~~~~~~~~~

        /**
         * @brief Gets index and value of the element that iterator refers to
         * @return Index and value of the element that iterator refers to
         */
        std::pair<IndexType, const T&> operator*() const;

        /**
         * @brief Gets index of the element that iterator refers to
         * @return Index of the element that iterator refers to
         */
        IndexType index() const;

        /**
         * @brief Gets value of the element that iterator refers to
         * @return Value of the element that iterator refers to
         */
        const T& value() const;

        // *** Mutators ***
        // ~~~~~~~~~~~~~~~~

        /**
         * @brief Goes to the next element
         * @return Iterator referring to the next element
         */
        ConstIterator& operator++();

        /**
         * @brief Goes to the next element
         * @return Iterator referring to the current element
         */
        const ConstIterator operator++(int);

        // *** Compare operators ***
        // ~~~~~~~~~~~~~~~~~~~~~~~~~

        bool operator==(const ConstIterator& other) const;
        bool operator!=(const ConstIterator& other) const;

    private:
        const MatrixType* m_instance;
        std::optional<typename SubMatrixType::ConstIterator> m_sub_iterator;
        typename StorageType::const_iterator m_current_iterator;
    };

private:
    StorageType m_values;
    T m_default_value{};
};


// ***************************************************************************
//              MATRIX CLASS PARTIAL SPECIALIZATION DECLARATION
// ***************************************************************************

template <typename T,
          typename Allocator>
class Matrix<T, 0, Allocator>
{
public:
    friend Matrix<T, 1, Allocator>;

    // *** Member types ***
    // ~~~~~~~~~~~~~~~~~~~~

    using ValueType  = T;
    using MatrixType = Matrix<T, 0, Allocator>;
    using IndexType  = std::array<std::size_t, 0>;

    // *** Constructors ***
    // ~~~~~~~~~~~~~~~~~~~~

    explicit Matrix(const T& default_value);

    Matrix() = default;
    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;
    Matrix(Matrix&&) noexcept = default;
    Matrix& operator=(Matrix&&) noexcept = default;

    /**
     * @brief Copies a value to 0-dimensional matrix
     * @param value Value to copy in the matrux
     * @return Reference to the matrix
     */
    Matrix& operator=(const T& value);

    /**
     * @brief Moves a value to 0-dimensional matrix
     * @param value Value to move in the matrux
     * @return Reference to the matrix
     */
    Matrix& operator=(T&& value);

    // *** Observers ***
    // ~~~~~~~~~~~~~~~~~

    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] bool empty() const;

    // *** Implicit-cast operators ***
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * @brief Implicitly casts 0-dimensional matrix to non-const l-value reference of type `T`
     */
    operator T&();

    /**
     * @brief Implicitly casts 0-dimensional matrix to const l-value reference of type `T`
     */
    operator const T&() const;

    // *** Index access ***
    // ~~~~~~~~~~~~~~~~~~~~

    T& at(const IndexType&);
    const T& at(const IndexType&) const;

private:
    template <typename IndexIterator>
    T& at(IndexIterator, IndexIterator);
    template <typename IndexIterator>
    const T& at(IndexIterator, IndexIterator) const;

public:
    // *** Iterators ***
    // ~~~~~~~~~~~~~~~~~

    class Iterator;
    class ConstIterator;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

    class Iterator
    {
    private:
        enum class Type { BEGIN, END, };
        friend MatrixType;
        Iterator(MatrixType* instance, Type type);

    public:
        // *** Observers ***
        // ~~~~~~~~~~~~~~~~~

        std::pair<IndexType, T&> operator*() const;
        IndexType index() const;
        T& value() const;

        // *** Mutators ***
        // ~~~~~~~~~~~~~~~~

        Iterator& operator++();
        const Iterator operator++(int);

        // *** Compare operators ***
        // ~~~~~~~~~~~~~~~~~~~~~~~~~

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        MatrixType* m_instance;
        Type m_type;
    };

    class ConstIterator
    {
    private:
        enum class Type { BEGIN, END, };
        friend MatrixType;
        ConstIterator(const MatrixType* instance, Type type);

    public:
        // *** Observers ***
        // ~~~~~~~~~~~~~~~~~

        std::pair<IndexType, const T&> operator*() const;
        IndexType index() const;
        const T& value() const;

        // *** Mutators ***
        // ~~~~~~~~~~~~~~~~

        ConstIterator& operator++();
        const ConstIterator operator++(int);

        // *** Compare operators ***
        // ~~~~~~~~~~~~~~~~~~~~~~~~~

        bool operator==(const ConstIterator& other) const;
        bool operator!=(const ConstIterator& other) const;

    private:
        const MatrixType* m_instance;
        Type m_type;
    };

private:
    T m_value{};
    T m_default_value{};
};


// ***************************************************************************
//                     PRIMARY MEMBER FUNCTION DEFINITION
// ***************************************************************************

#define TEMPLATE_ARGS       template <typename T, std::size_t dimensions, typename Allocator>
#define MATRIX_TYPE         Matrix<T, dimensions, Allocator>

// *** Constructors ***
// ~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
MATRIX_TYPE::Matrix(T default_value)
    : m_default_value(std::move(default_value))
{
}


// *** Observers ***
// ~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
std::size_t MATRIX_TYPE::size() const
{
    std::size_t answer = 0;
    for (auto& [index, value] : m_values)
        answer += value.size();
    return answer;
}

TEMPLATE_ARGS
bool MATRIX_TYPE::empty() const
{
    return size() == 0;
}


// *** Mutators ***
// ~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
void MATRIX_TYPE::shrink_to_fit()
{
    for (auto it = m_values.begin(); it != m_values.end();)
    {
        if (it->second.size() == 0)
            it = m_values.erase(it);
        else
            ++it;
    }
}


// *** Index access ***
// ~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
typename MATRIX_TYPE::SubMatrixType& MATRIX_TYPE::operator[](std::size_t index)
{
    auto it = m_values.find(index);
    if (it != m_values.end())
        return it->second;
    auto [emplaced, _] = m_values.emplace(index, Matrix<T, dimensions - 1, Allocator>(m_default_value));
    return emplaced->second;
}

TEMPLATE_ARGS
const typename MATRIX_TYPE::SubMatrixType& MATRIX_TYPE::operator[](std::size_t index) const
{
    auto it = m_values.find(index);
    if (it != m_values.end())
        return it->second;
    auto [emplaced, _] = m_values.emplace(index, Matrix<T, dimensions - 1, Allocator>(m_default_value));
    return emplaced->second;
}

TEMPLATE_ARGS
T& MATRIX_TYPE::at(const IndexType& index)
{
    return operator[](index[0]).at(index.data() + 1, index.data() + index.size());
}

TEMPLATE_ARGS
const T& MATRIX_TYPE::at(const IndexType& index) const
{
    return operator[](index[0]).at(index.data() + 1, index.data() + index.size());
}

TEMPLATE_ARGS
template <typename IndexIterator>
T& MATRIX_TYPE::at(IndexIterator begin, IndexIterator end)
{
    return operator[](*begin).at(std::next(begin), end);
}


TEMPLATE_ARGS
template <typename IndexIterator>
const T& MATRIX_TYPE::at(IndexIterator begin, IndexIterator end) const
{
    return operator[](*begin).at(std::next(begin), end);
}


// *** Constant Iterator creation functions ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
typename MATRIX_TYPE::ConstIterator MATRIX_TYPE::begin() const
{
    return ConstIterator(this, ConstIterator::Type::BEGIN);
}

TEMPLATE_ARGS
typename MATRIX_TYPE::ConstIterator MATRIX_TYPE::end() const
{
    return ConstIterator(this, ConstIterator::Type::END);
}


// *** Constant Iterator constructors ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
MATRIX_TYPE::ConstIterator::ConstIterator(const MatrixType* instance, Type type)
    : m_instance(instance)
{
    switch (type)
    {
    case Type::BEGIN:
        m_current_iterator = m_instance->m_values.cbegin();
        if (m_instance->m_values.begin() == m_instance->m_values.cend())
        {
            m_sub_iterator = std::nullopt;
        }
        else
        {
            m_sub_iterator = m_current_iterator->second.begin();
        }
        break;
    case Type::END:
        m_current_iterator = m_instance->m_values.cend();
        m_sub_iterator = std::nullopt;
        break;
    }
}


// *** Constant Iterator observers ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
std::pair<typename MATRIX_TYPE::IndexType, const T&> MATRIX_TYPE::ConstIterator::operator*() const
{
    std::pair<IndexType, const T&> p(index(), value());
    return p;
}

TEMPLATE_ARGS
typename MATRIX_TYPE::IndexType MATRIX_TYPE::ConstIterator::index() const
{
    IndexType answer;
    answer[0] = m_current_iterator->first;
    SubIndexType subindex = m_sub_iterator.value().index();
    for (int i = 0; i < subindex.size(); ++i)
        answer[i + 1] = subindex[i];
    return answer;
}

TEMPLATE_ARGS
const T& MATRIX_TYPE::ConstIterator::value() const
{
    const T& ans = m_sub_iterator.value().value();
    return ans;
}


// *** Constant Iterator mutators ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
typename MATRIX_TYPE::ConstIterator& MATRIX_TYPE::ConstIterator::operator++()
{
    auto job = [this]() -> void
    {
        ++m_sub_iterator.value();
        while (m_sub_iterator == m_current_iterator->second.end())
        {
            ++m_current_iterator;
            if (m_current_iterator == m_instance->m_values.end())
            {
                m_sub_iterator = std::nullopt;
                return;
            }
            m_sub_iterator = m_current_iterator->second.begin();
        }
    };

    do
    {
        job();
    } while (m_sub_iterator != std::nullopt && value() == m_instance->m_default_value);

    return *this;
}

TEMPLATE_ARGS
const typename MATRIX_TYPE::ConstIterator MATRIX_TYPE::ConstIterator::operator++(int)
{
    ConstIterator old = *this;
    ++(*this);
    return old;
}


// *** Constant Iterator compare operators ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
bool MATRIX_TYPE::ConstIterator::operator==(const MATRIX_TYPE::ConstIterator& other) const
{
    return std::tie(m_instance, m_sub_iterator, m_current_iterator) ==
           std::tie(other.m_instance, other.m_sub_iterator, other.m_current_iterator);
}

TEMPLATE_ARGS
bool MATRIX_TYPE::ConstIterator::operator!=(const MATRIX_TYPE::ConstIterator& other) const
{
    return !(*this == other);
}


// *** Iterator creation functions ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
typename MATRIX_TYPE::Iterator MATRIX_TYPE::begin()
{
    return ConstIterator(this, Iterator::Type::BEGIN);
}

TEMPLATE_ARGS
typename MATRIX_TYPE::Iterator MATRIX_TYPE::end()
{
    return ConstIterator(this, Iterator::Type::END);
}


// *** Iterator constructors ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
MATRIX_TYPE::Iterator::Iterator(MatrixType* instance, Type type)
    : m_instance(instance)
{
    switch (type)
    {
    case Type::BEGIN:
        m_current_iterator = m_instance->m_values.cbegin();
        if (m_instance->m_values.begin() == m_instance->m_values.cend())
        {
            m_sub_iterator = std::nullopt;
        }
        else
        {
            m_sub_iterator = m_current_iterator->second.begin();
        }
        break;
    case Type::END:
        m_current_iterator = m_instance->m_values.cend();
        m_sub_iterator = std::nullopt;
        break;
    }
}


// *** Iterator observers ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
std::pair<typename MATRIX_TYPE::IndexType, T&> MATRIX_TYPE::Iterator::operator*() const
{
    std::pair<IndexType, T&> p(index(), value());
    return p;
}

TEMPLATE_ARGS
typename MATRIX_TYPE::IndexType MATRIX_TYPE::Iterator::index() const
{
    IndexType answer;
    answer[0] = m_current_iterator->first;
    SubIndexType subindex = m_sub_iterator.value().index();
    for (int i = 0; i < subindex.size(); ++i)
        answer[i + 1] = subindex[i];
    return answer;
}

TEMPLATE_ARGS
T& MATRIX_TYPE::Iterator::value() const
{
    const T& ans = m_sub_iterator.value().value();
    return ans;
}


// *** Iterator mutators ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
typename MATRIX_TYPE::Iterator& MATRIX_TYPE::Iterator::operator++()
{
    auto job = [this]() -> void
    {
        ++m_sub_iterator.value();
        while (m_sub_iterator == m_current_iterator->second.end())
        {
            ++m_current_iterator;
            if (m_current_iterator == m_instance->m_values.end())
            {
                m_sub_iterator = std::nullopt;
                return;
            }
            m_sub_iterator = m_current_iterator->second.begin();
        }
    };

    do
    {
        job();
    } while (m_sub_iterator != std::nullopt && value() == m_instance->m_default_value);

    return *this;
}

TEMPLATE_ARGS
const typename MATRIX_TYPE::Iterator MATRIX_TYPE::Iterator::operator++(int)
{
    Iterator old = *this;
    ++(*this);
    return old;
}


// *** Iterator compare operators ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS
bool MATRIX_TYPE::Iterator::operator==(const MATRIX_TYPE::Iterator& other) const
{
    return std::tie(m_instance, m_sub_iterator, m_current_iterator) ==
           std::tie(other.m_instance, other.m_sub_iterator, other.m_current_iterator);
}

TEMPLATE_ARGS
bool MATRIX_TYPE::Iterator::operator!=(const MATRIX_TYPE::Iterator& other) const
{
    return !(*this == other);
}


#undef TEMPLATE_ARGS
#undef MATRIX_TYPE

// ***************************************************************************
//             PARTIAL SPECIALIZATION MEMBER FUNCTION DEFINITION
// ***************************************************************************

#define TEMPLATE_ARGS_0D    template <typename T, typename Allocator>
#define MATRIX_TYPE_0D      Matrix<T, 0, Allocator>

// *** Constructors ***
// ~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
MATRIX_TYPE_0D::Matrix(const T& default_value)
    : m_value(default_value)
    , m_default_value(default_value)
{
}

TEMPLATE_ARGS_0D
MATRIX_TYPE_0D& MATRIX_TYPE_0D::operator=(const T& value)
{
    m_value = value;
    return *this;
}

TEMPLATE_ARGS_0D
MATRIX_TYPE_0D& MATRIX_TYPE_0D::operator=(T&& value)
{
    m_value = std::move(value);
    return *this;
}


// *** Observers ***
// ~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
std::size_t MATRIX_TYPE_0D::size() const
{
    return (m_value == m_default_value ? 0 : 1);
}

TEMPLATE_ARGS_0D
bool MATRIX_TYPE_0D::empty() const
{
    return size() == 0;
}


// *** Implicit-cast operators ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
MATRIX_TYPE_0D::operator const T&() const
{
    return m_value;
}

TEMPLATE_ARGS_0D
MATRIX_TYPE_0D::operator T&()
{
    return m_value;
}


// *** Index access ***
// ~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
T& MATRIX_TYPE_0D::at(const IndexType&)
{
    return m_value;
}

TEMPLATE_ARGS_0D
template <typename IndexIterator>
T& MATRIX_TYPE_0D::at(IndexIterator, IndexIterator)
{
    return m_value;
}


// *** Constant Iterator creation functions ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
typename MATRIX_TYPE_0D::ConstIterator MATRIX_TYPE_0D::begin() const
{
    return ConstIterator(this, ConstIterator::Type::BEGIN);
}

TEMPLATE_ARGS_0D
typename MATRIX_TYPE_0D::ConstIterator MATRIX_TYPE_0D::end() const
{
    return ConstIterator(this, ConstIterator::Type::END);
}


// *** Constant Iterator constructors ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
MATRIX_TYPE_0D::ConstIterator::ConstIterator(const MatrixType* instance, Type type)
    : m_instance(instance)
    , m_type(type)
{
}


// *** Constant Iterator observers ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
std::pair<typename MATRIX_TYPE_0D::IndexType, const T&> MATRIX_TYPE_0D::ConstIterator::operator*() const
{
    return std::make_pair(index(), value());
}

TEMPLATE_ARGS_0D
typename MATRIX_TYPE_0D::IndexType MATRIX_TYPE_0D::ConstIterator::index() const
{
    return {};
}

TEMPLATE_ARGS_0D
const T& MATRIX_TYPE_0D::ConstIterator::value() const
{
    return m_instance->m_value;
}


// *** Constant Iterator mutators ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
typename MATRIX_TYPE_0D::ConstIterator& MATRIX_TYPE_0D::ConstIterator::operator++()
{
    m_type = Type::END;
    return *this;
}

TEMPLATE_ARGS_0D
const typename MATRIX_TYPE_0D::ConstIterator MATRIX_TYPE_0D::ConstIterator::operator++(int)
{
    ConstIterator old = *this;
    ++(*this);
    return old;
}


// *** Constant Iterator compare operators ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
bool MATRIX_TYPE_0D::ConstIterator::operator==(const ConstIterator& other) const
{
    return std::tie(m_instance, m_type) == std::tie(other.m_instance, other.m_type);
}

TEMPLATE_ARGS_0D
bool MATRIX_TYPE_0D::ConstIterator::operator!=(const ConstIterator& other) const
{
    return !(*this == other);
}


// *** Iterator creation functions ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
typename MATRIX_TYPE_0D::Iterator MATRIX_TYPE_0D::begin()
{
    return ConstIterator(this, ConstIterator::Type::BEGIN);
}

TEMPLATE_ARGS_0D
typename MATRIX_TYPE_0D::Iterator MATRIX_TYPE_0D::end()
{
    return ConstIterator(this, ConstIterator::Type::END);
}


// *** Iterator constructors ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
MATRIX_TYPE_0D::Iterator::Iterator(MatrixType* instance, Type type)
    : m_instance(instance)
    , m_type(type)
{
}


// *** Iterator observers ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
std::pair<typename MATRIX_TYPE_0D::IndexType, T&> MATRIX_TYPE_0D::Iterator::operator*() const
{
    return std::make_pair(index(), value());
}

TEMPLATE_ARGS_0D
typename MATRIX_TYPE_0D::IndexType MATRIX_TYPE_0D::Iterator::index() const
{
    return {};
}

TEMPLATE_ARGS_0D
T& MATRIX_TYPE_0D::Iterator::value() const
{
    return m_instance->m_value;
}


// *** Iterator mutators ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
typename MATRIX_TYPE_0D::Iterator& MATRIX_TYPE_0D::Iterator::operator++()
{
    m_type = Type::END;
    return *this;
}

TEMPLATE_ARGS_0D
const typename MATRIX_TYPE_0D::Iterator MATRIX_TYPE_0D::Iterator::operator++(int)
{
    Iterator old = *this;
    ++(*this);
    return old;
}


// *** Iterator compare operators ***
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TEMPLATE_ARGS_0D
bool MATRIX_TYPE_0D::Iterator::operator==(const Iterator& other) const
{
    return std::tie(m_instance, m_type) == std::tie(other.m_instance, other.m_type);
}

TEMPLATE_ARGS_0D
bool MATRIX_TYPE_0D::Iterator::operator!=(const Iterator& other) const
{
    return !(*this == other);
}


#undef TEMPLATE_ARGS_0D
#undef MATRIX_TYPE_0D

} // namespace my

#endif // MATRIX_H
