#include "Header.hpp"


// для инференса тензоры
int main()
{



    return 0;
}


// нормальное копирование сделать + правило 5
// выбор хранения row/column
// shared_ptr -- не копирование а передача указателя прост, для очистки есть счетчик enable_shared_from_thi
// аллокатор свой написаь с раздачей памяти? передавать в конструктор кусок нужный? или обертку сделать на вызов new?????
// 


inline constexpr std::size_t GEOM_ALIGNMENT = 16;  // alignment of basic geometry structures
template <std::size_t Dimension, bool HowSave>
class alignas(GEOM_ALIGNMENT) Tensor
{
    private:
        precision_t* all_el_{};



    private:
        Tensor(is_Decard_dim a)
        {

        }

        Tensor()
        {}

        ~Tensor()
        {}

        constexpr const precision_t determinant () const noexcept requires (hight_ ==  width_)
        {
            return 23;
        }


};
