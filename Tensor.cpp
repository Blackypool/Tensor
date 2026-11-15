#include "Header.hpp"

//________________________________________________Questions_Tasks__________________________________________________________________________//
// статический анализ графа для выделения нужного количества памяти аллокатором

// нормальное копирование сделать + правило 5
// выбор хранения row/column
// shared_ptr -- не копирование а передача указателя прост, для очистки есть счетчик enable_shared_from_thi
// аллокатор свой написаь с раздачей памяти? передавать в конструктор кусок нужный? или обертку сделать на вызов new?????
// 

//_________________________________________________________________________________________________________________________________________//

// для инференса тензоры
int main()
{



    return 0;
}



// inline constexpr std::size_t GEOM_ALIGNMENT = 16;


// class alignas(GEOM_ALIGNMENT) Tensor
enum class Approach { Row_Major, Column_Major };

template <Approach Way = Approach::Row_Major, std::size_t Dimension = 6>
class Tensor
{
    private:
        std::shared_ptr <precision_t[]> all_el_; // size is give out of full pool.     {}????//x/xz/

        std::size_t shape_[Dimension]{};          // [Batch, Channels, Height, Width...] in any order u need
        std::size_t strides_[Dimension]{};

        constexpr calc_strides()
        {}

        constexpr Tensor(std::shared_ptr <precision_t[]> list, std::vector format) noexcept : all_el_(std::move(list))
        {
            if constexpr ()
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

    public:

        constexpr precision_t get_one_elem (std::initializer_list <size_t> whr) noexcept // noexcept???? а вдруг за рамки массива??? + добавить провереку на размер where или не надо и похуй так как если че похуй
        {
            const std::size_t* where = whr.begin();
            std::size_t index = 0;

            for (std::size_t i = 0; i < Dimension; i++)
                index += where[i] * strides_[i];

            return all_el_[index];
        }
// копирование всего пришедшего массива
// несколько фабрикъ
// получить строку
// получить столбец
// возвращать указатель на первый + страйдера как чекать некст элемент нужной строки/столбца
// транспонирование
//
Row-Major
Column-Major

        Tensor()
        {

        }
        
        template <is_Decard_dim TypeCoord>
        constexpr Tensor(TypeCoord* list, std::size_t format) noexcept
        {
            // 
            all_el_ = std::move(list);

        }


        template <is_Decard_dim TypeCoord>
        constexpr Tensor(std::initializer_list <TypeCoord> list) noexcept
        {
            std::size_t i = 0;
            for (const TypeCoord& val : list)
                    all_coords_[i++] = static_cast<precision_t>(val);
        }

        ~Tensor()
        {}

        // транспонирование
        // 
        constexpr const precision_t determinant () const noexcept requires (hight_ ==  width_)
        {
            return 23;
        }


};
