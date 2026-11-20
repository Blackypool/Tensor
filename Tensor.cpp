#include "Header.hpp"

//________________________________________________Questions_Tasks__________________________________________________________________________//
// статический анализ графа для выделения нужного количества памяти аллокатором

// аллокатор свой написаь с раздачей памяти? передавать в конструктор кусок нужный? или обертку сделать на вызов new?????
//_________________________________________________________________________________________________________________________________________//


int main()
{

    return 0;
}


//_______________________________________Class for any came shape format, that convert into []_____________________________________________//
template <std::size_t Dimension = 6>
class Shape
{
    private:
        std::size_t shape_[Dimension]{};
    
    public:
        #define FILL_SHAPE for (std::size_t i = 0; i < Dimension; i++)\
                                shape_[i] = static_cast<precision_t>(format[i]);

        // initializer_list //
        template <is_Decard_dim Shape_type>
        constexpr Shape(std::initializer_list <Shape_type> format) noexcept // need proverka na size of input parametr
        {
            std::size_t i = 0;
            for (const Shape_type& val : format)
                shape_[i++] = static_cast<precision_t>(val);
        }
        
        // list //
        template <is_Decard_dim Shape_type>
        constexpr Shape(std::list<Shape_type> format) noexcept // need proverka na size of input parametr
        {
            for (auto val : format)
                shape_[i] = static_cast<precision_t>(val);
        }

        // vector <bool> //
        constexpr Shape(std::vector <bool> format)      noexcept { FILL_SHAPE }

        // T* //
        template <is_Decard_dim Shape_type>
        constexpr Shape(Shape_type* format)             noexcept { FILL_SHAPE }

        // another all w\ span<T> //
        template <is_Decard_dim Shape_type> // need proverka na size of input parametr
        constexpr Shape(std::span<Shape_type> format)   noexcept { FILL_SHAPE }

        #undef FILL_SHAPE
};
//_________________________________________________________________________________________________________________________________________//


//_________________________________________________________________________________________________________________________________________//
class Storage
{




};
//_________________________________________________________________________________________________________________________________________//


//___________________________________Comfortable work w\ column elements when row-major used_______________________________________________//
class Strided_it
{
    private:
        precision_t* ptr_;
        std::size_t stride_;

    public:
        constexpr Strided_it(precision_t* ptr, std::size_t step) noexcept : ptr_(ptr), stride_(step) {}

        constexpr precision_t operator*() noexcept
        {
            return *ptr_;
        }
                
        constexpr precision_t operator[](std::size_t i) noexcept//?
        {
            return ptr_[i * stride_];
        }

        constexpr Strided_it operator++() noexcept
        {
            ptr_ = ptr_ + stride_;
            return *this;
        }
                
        constexpr Strided_it operator--() noexcept
        {
            ptr_ = ptr_ - stride_;
            return *this;
        }
};
//_________________________________________________________________________________________________________________________________________//


//________________________________________________________TENSOR___________________________________________________________________________//
enum class Approach { Row_Major, Column_Major };

template <Approach Way = Approach::Row_Major, std::size_t Dimension = 6>
class alignas(GEOM_ALIGNMENT) Tensor
{
    private:
        std::shared_ptr <precision_t[]> all_el_; // size is give out of full pool.     {}????//x/xz/ можно передать лямбда функцию чтобы та брала куски из моей памяти по нужному размеру с помощью хеш таблицы и списков? по размерам типа

        std::size_t shape_[Dimension]{};         // [Batch, Channels, Height, Width...] in any order u need, but how work striders is choose by Approach Way
        std::size_t strides_[Dimension]{};

        Device device_;                          // not template, because heterogeneous execution

        //______________________________________________//
        constexpr Tensor (Storage storage, Shape format) noexcept : all_el_(std::move(storage))
        {
            if constexpr (Way == Row_Major)
            {
                std::size_t cur_stride = 1;
                for (int i = (static_cast<int>)Dimension; i >= 0; i--)
                {
                    shape_[i]   = format[i];

                    strides_[i] = cur_stride;
                    cur_stride  = shape_[i] * cur_stride;
                }
            }
            else
            {
                std::size_t cur_stride = 1;
                for (int i = (static_cast<int>)Dimension; i >= 0; i--)
                {
                    shape_[i]   = format[i];

                    strides_[Dimension - i] = cur_stride;
                    cur_stride  = shape_[i] * cur_stride;
                }
            }
        }
//_________________________________________________________________________________________________________________________________________//
    public:

        //________________Constructors__________________//
        constexpr Tensor trash (Shape shape_format)  // new memory w\ not init data
        {
            auto ptr_to_all = std::make_shared<precision_t[]>( size(shape_format) ); // in future here will go allocator func by myself

            return Tensor(ptr_to_all, shape_format);
        }

        constexpr Tensor zeros (Shape shape_format)  // new memory w\ all zero el-s
        {
            std::size_t size_all = size(shape_format);
            auto ptr_to_all = std::make_shared<precision_t[]>(size_all); // in future here will go allocator func by myself

            for (std::size_t i = 0; i < size_all; i++)
                ptr_to_all[i] = 0;

            return Tensor(ptr_to_all, shape_format);
        }

        constexpr Tensor full_copy (Storage storage, Shape shape_format)  // new memory w\ full copy out of ptr
        {
            std::size_t size_all = size(shape_format);
            auto ptr_to_all = std::make_shared<precision_t[]>(size_all);

            for (std::size_t i = 0; i < size_all; i++)
                ptr_to_all[i] = storage[i];

            return Tensor(ptr_to_all, shape_format);
        }

        constexpr Tensor move_ownership (Storage storage, Shape shape_format)  // new memory w\ move come data
        {
            std::shared_ptr<int> ptr_to_all(storage);

            return Tensor(ptr_to_all, shape_format);
        }

        //__________________getters_____________________//
        constexpr std::size_t size (Shape shape_format) noexcept const
        {
            std::size_t size_all = 1;
            for (std::size_t i = 0; i < Dimension; i++)
                size_all = size_all * shape_format[i];
            
            return size_all;
        }

        constexpr precision_t one_elem (Shape whr) noexcept // noexcept???? а вдруг за рамки массива??? + добавить провереку на размер where или не надо и похуй так как если че похуй
        {
            const std::size_t* where = whr.begin();
            std::size_t index = 0;

            for (std::size_t i = 0; i < Dimension; i++)
                index += where[i] * strides_[i];

            return all_el_[index];
        }

        constexpr auto row (std::size_t whr) noexcept
        {
            if constexpr (Way == Row_Major)
            {
                return &(all_el_[whr])
            }
            else
            {
                Strided_it ptr_w_strides(&(all_el_[whr]), strides_[Dimension - 1]);
                return ptr_w_strides;
            }
        }

        constexpr auto col (std::initializer_list <size_t> whr) noexcept
        {
            if constexpr (Way == Row_Major)
            {
                Strided_it ptr_w_strides(&(all_el_[whr]), strides_[Dimension - 1]);
                return ptr_w_strides;
            }
            else
            {
                return &(all_el_[whr])
            }
        }

        //__________________operations__________________//
        constexpr Tensor& self_transpose () noexcept
        {
            if constexpr (Way == Row_Major)
            {
                std::size_t old_row = shape_[Dimension - 1];
                shape_[Dimension - 1] = shape_[Dimension];
                shape_[Dimension] = old_row;

                old_row = strides_[Dimension - 1];
                strides_[Dimension - 1] = strides_[Dimension];
                strides_[Dimension] = old_row;
            }
            else
            {
                std::size_t old_row = shape_[0];
                shape_[0] = shape_[1];
                shape_[1] = old_row;

                old_row = strides_[0];
                strides_[0] = strides_[1];
                strides_[1] = old_row;
            }

            return *this;
        }

        constexpr Tensor ret_transpose () noexcept const
        {
            if constexpr (Way == Row_Major)
            {
                std::size_t old_row = shape_[Dimension - 1];
                shape_[Dimension - 1] = shape_[Dimension];
                shape_[Dimension] = old_row;

                old_row = strides_[Dimension - 1];
                strides_[Dimension - 1] = strides_[Dimension];
                strides_[Dimension] = old_row;

                // функция копирования из правила пяти
            }
            else
            {
                std::size_t old_row = shape_[0];
                shape_[0] = shape_[1];
                shape_[1] = old_row;

                old_row = strides_[0];
                strides_[0] = strides_[1];
                strides_[1] = old_row;

                // функция копирования из правила пяти
            }
        }

        constexpr Tensor determinant () noexcept const
        {

        }

        // функция разреза на разные девайсы без копирования 
        // функция перекидки на другое устройство

        // просто функция решейп или какая-то которая режет
        
        // создание по девайсу

        // принятие любого типа данных в конструктор
};
//_________________________________________________________________________________________________________________________________________//