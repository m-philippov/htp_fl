#pragma once
#include <stdexcept>
#include <cassert>

//Задание 2 (30 баллов) : Напишите библиотеку, реализующую шаблонный класс для 
// односвязного списка с поддержкой операций вставки, удаления и поиска заданного элемента.

namespace htp {
    /*
    class WrongExpressionError : public std::out_of_range {
    public:
        using std::out_of_range::out_of_range;

    };
    */
    template<typename T>
    class forward_list  {
    public:
        forward_list() = default;
        //forward_list(const forward_list& list);
        ~forward_list() noexcept;
    private:      
        class node {
        public:
            //node() = default;
            //noexcept для компилятора (более эффективный код) - метод никогда не создает исключение
            //перемещение для эффективности
            //чтобы не вызвался конструктор по сслылке вместо перемещения
            node(const node& data) = delete;
            node(T&& data) noexcept : data_(data), next_(nullptr){}
            //оператор присваивания
            node& operator=(const node& data) = delete;
            node& operator=(node&& data) noexcept {
                data_ = data.data_;
                data.next_ = nullptr;
                return *this;
            }
            node* next_;
            T data_;
        };
        //Поведенческий паттерн проектирования итератор (Behavioral pattern iterator)
        // содержит операторы сравнения (либо ==, либо !=), инкремента и разыменования
    public:
        class node_iterator {
        public:
            //доступ к приватным полям
            friend class node;
            explicit node_iterator(node* ptr = nullptr) noexcept : current_(ptr) {};
            bool operator==(const node_iterator& it) const noexcept {
                return this->current_ == it.current_;
            }
            bool operator!=(const node_iterator& it) const noexcept {
                //std::cout << (this->current_ != it.current_) << '\t';
                return this->current_ != it.current_;
            }
            node_iterator& operator++() {
                if (current_) {
                    current_ = current_->next_;
                    return *this;
                }
                else throw std::out_of_range("iterator int ++");
            }
            node_iterator operator++(int) {
                if (current_) {
                    auto copy = *this;
                    current_ = current_;
                    return copy;
                }
                else throw std::out_of_range("iterator int ++");
            }
            const T& operator*() const {
                return current_ ? current_->data_ : throw std::out_of_range("const iterator *");
            }
            T& operator*()
            {
                return current_ ? current_->data_ : throw std::out_of_range("iterator *");
            }
            node* get() noexcept
            {
                return current_;
            }
        private:
            node* current_;
        };
    private:
        node* first_ = nullptr;



        //"рекурсивный" вызов для добавления нескольких элементов
        //"рекурсивный" в ковычках, так как каждый раз новая функция
        template<typename Arg>
        void _insert_after(node_iterator pos, Arg&& arg) {
            auto ptr = pos.get();
            if (!ptr) {
                //if (!first_) {
                //    push_front(std::move(arg));
                //}
                //else {
                    throw std::out_of_range("_insert_after");
                //}
            }
            else {
                auto newnode = new node{ std::move(arg) };
                newnode->next_ = ptr->next_;
                ptr->next_ = newnode;
            }
            //return pos;
        }
        template<typename Arg, typename... Args>
        void _insert_after(node_iterator pos, Arg&& arg, Args&&... args)  {
            auto ptr = pos.get();
            if (!ptr) {
                //if (!first_) {
                //    push_front(std::move(arg));
                //    _insert_after(++pos, args...);
                //}
                //else {
                    throw std::out_of_range("_insert_after");
                //}
            }
            else {
                auto newnode = new node{ std::move(arg) };
                newnode->next_ = ptr->next_;
                ptr->next_ = newnode;
                _insert_after(++pos, args...);
            }
            //return pos;
        }
        
    public:
        bool empty() const noexcept;
        //итераторы чтения
        [[__nodiscard__]]
        const auto begin() const noexcept;
        [[__nodiscard__]]
        const auto end() const noexcept;
        //итераторы записи
        [[__nodiscard__]]
        auto begin() noexcept;
        [[__nodiscard__]]
        auto end() noexcept;

        //добавление в начало за О(1) с потерей ссылки
        void push_front(T&& data) noexcept;

        //добавление в начало за О(1) без потери ссылки
        void push_front(const T& data) noexcept;

        //добавление в начало за О(pos) c потерей ссылки
        void insert_after(node_iterator pos, T&& data);

        //добавление в начало за О(pos) без потери ссылки
        void insert_after(node_iterator pos, const T& data);
        
        //удаление следующего за pos элемента за O(pos)
        void erase_after(node_iterator pos);

        //удаление первого элемента
        void pop_front();

        //поиск за О(pos)
        auto find(const T& data);



    };

}
//template<typename T>
//inline htp::forward_list<T>::forward_list(const forward_list& list) {

//}
template<typename T>
inline htp::forward_list<T>::~forward_list() noexcept {
    while (first_) {
        pop_front();
    }
}
template<typename T>
inline bool htp::forward_list<T>::empty() const noexcept {
    return this->first_ == nullptr;
}

template <typename T>
inline const auto htp::forward_list<T>::begin() const noexcept {
    return node_iterator { this->first_};
}
template <typename T>
inline const auto htp::forward_list<T>::end() const noexcept {
    return node_iterator { nullptr };
}

template <typename T>
inline auto htp::forward_list<T>::begin() noexcept {
    return node_iterator { this->first_};
}
template <typename T>
inline auto htp::forward_list<T>::end() noexcept {
    return node_iterator{ nullptr };
}
template<typename T>
inline void htp::forward_list<T>::push_front(T&& data) noexcept {
    if (this->first_ == nullptr) {
        this->first_ = new node{ std::move(data) };
    }
    else {
        auto newnode = new node{std::move(data)};
        newnode->next_ = first_;
        first_ = newnode;
    }
}
template<typename T>
inline void htp::forward_list<T>::push_front(const T& data) noexcept {
    if (this->first_ == nullptr) {
        this->first_ = new node{ data };
    }
    else {
        auto newnode = new node{ data };
        newnode->next_ = first_;
        first_ = newnode;
    }
}
template<typename T>
inline void htp::forward_list<T>::insert_after(htp::forward_list<T>::node_iterator pos, T&& data) {
     _insert_after(pos, std::move(data));
}
template<typename T>
inline void htp::forward_list<T>::insert_after(htp::forward_list<T>::node_iterator pos, const T& data) {
    _insert_after(pos, data);
}
template<typename T>
inline void htp::forward_list<T>::erase_after(node_iterator pos) {
    auto ptr = pos.get();
    if (ptr) {
        auto nodeToDelete = ptr->next_;
        if (ptr->next_)
        {
            ptr->next_ = ptr->next_->next_;
        }
        delete nodeToDelete;
    } else {
        throw std::out_of_range("erase_after");
    }
}
template<typename T>
inline void htp::forward_list<T>::pop_front() {
    auto nodeToDelete = first_; 
    if (nodeToDelete) {
        first_ = first_->next_;
        delete nodeToDelete;
    }
    else {
        throw std::out_of_range("popfront");
    }
}
template<typename T>
inline auto htp::forward_list<T>::find(const T& data)
{
    for (auto it = begin(); it != end(); ++it)
    {
        if (*it == data)
        {
            return it;
        }
    }

    return node_iterator{ nullptr };
}
