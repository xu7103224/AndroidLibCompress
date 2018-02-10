/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#define nullptr 0

template<typename T>
struct LinkedListEntry {
  LinkedListEntry<T>* next;
  T* element;
};

/*
 * Represents linked list of objects of type T
 */
template<typename T, typename Allocator>
class LinkedList {
 public:
  LinkedList() : head_(nullptr), tail_(nullptr) {}
  ~LinkedList() {
    clear();
  }

  void push_front(T* const element) {
    LinkedListEntry<T>* new_entry = Allocator::alloc();
    new_entry->next = head_;
    new_entry->element = element;
    head_ = new_entry;
    if (tail_ == nullptr) {
      tail_ = new_entry;
    }
  }

  void push_back(T* const element) {
    LinkedListEntry<T>* new_entry = Allocator::alloc();
    new_entry->next = nullptr;
    new_entry->element = element;
    if (tail_ == nullptr) {
      tail_ = head_ = new_entry;
    } else {
      tail_->next = new_entry;
      tail_ = new_entry;
    }
  }

  T* pop_front() {
    if (head_ == nullptr) {
      return nullptr;
    }

    LinkedListEntry<T>* entry = head_;
    T* element = entry->element;
    head_ = entry->next;
    Allocator::free(entry);

    if (head_ == nullptr) {
      tail_ = nullptr;
    }

    return element;
  }

  void clear() {
    while (head_ != nullptr) {
      LinkedListEntry<T>* p = head_;
      head_ = head_->next;
      Allocator::free(p);
    }

    tail_ = nullptr;
  }

  template<typename F>
  void for_each(F action) {
    visit([&] (T* si) {
      action(si);
      return true;
    });
  }

  template<typename F>
  bool visit(F action) {
    for (LinkedListEntry<T>* e = head_; e != nullptr; e = e->next) {
      if (!action(e->element)) {
        return false;
      }
    }
    return true;
  }

  template<typename F>
  void remove_if(F predicate) {
    for (LinkedListEntry<T>* e = head_, *p = nullptr; e != nullptr;) {
      if (predicate(e->element)) {
        LinkedListEntry<T>* next = e->next;
        if (p == nullptr) {
          head_ = next;
        } else {
          p->next = next;
        }
        Allocator::free(e);
        e = next;
      } else {
        p = e;
        e = e->next;
      }
    }
  }

  size_t copy_to_array(T* array[], size_t array_length) const {
    size_t sz = 0;
    for (LinkedListEntry<T>* e = head_; sz < array_length && e != nullptr; e = e->next) {
      array[sz++] = e->element;
    }

    return sz;
  }

  bool contains(const T* el) const {
    for (LinkedListEntry<T>* e = head_; e != nullptr; e = e->next) {
      if (e->element == el) {
        return true;
      }
    }
    return false;
  }

 private:
  LinkedListEntry<T>* head_;
  LinkedListEntry<T>* tail_;
//  DISALLOW_COPY_AND_ASSIGN(LinkedList);
};

#endif // __LINKED_LIST_H
