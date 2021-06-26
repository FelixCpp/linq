# linq
A small library to create language integrated queries in C++

#### Enumerable (methods):

+ Aggregate functions

  + aggregate

  + min

  + max

  + avg

  + sum

  + count

    

+ Retrieving specific elements

  + first

  + first_or_default

  + last

  + last_or_default

  + element_at

    

+ Advanced query

  + shuffle

  + order_by, then_by

  + pairwise

  + skip, skip_while

  + take, take_while

  + select, select_many

  + join

  + intersect_with

  + union_with

  + except

    

+ Conversions

  + to_lookup

  + to_list

  + to_vector

  + to_queue

  + to_stack

  + to_set

  + to_map

  + cast

    

+ item existence

  + all
  + any
  + where
  + sequence_equal
  + contains



#### Creating ranges

- repeat (value, amount)
- range (start, end, increment)
- from (array)
- from (stl-container)
- from (begin, end)
