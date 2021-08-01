#include "growable_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

struct Date {
  int year;
  int month;
  int day;
};

static int date_cmp(const void* lhs_vp, const void* rhs_vp) {
  const struct Date* lhs = lhs_vp;
  const struct Date* rhs = rhs_vp;
  return lhs->year < rhs->year ? -1
    : lhs->year > rhs->year ? 1
    : lhs->month < rhs->month ? -1
    : lhs->month > rhs->month ? 1
    : lhs->day < rhs->day ? -1
    : lhs->day > rhs->day ? 1
    : 0;
}

static int is_covid_year(void* date_vp) {
    struct Date* date = date_vp;
    if(date->year > 2019 && date->year < 2022) {
        return 1;
    }
    
    return -1;
}

static struct Date* create_date(int year, int month, int day) {
  struct Date* d = malloc(sizeof *d);
  *d = (struct Date){.year=year, .month=month, .day=day};
  return d;
}

static void* duplicate_date(const void* date_vp) {
  const struct Date* date = date_vp;
  return create_date(date->year, date->month, date->day);
}

static void print_date(void* date) {
    const struct Date* date_ = date;
    printf("%d-%d-%d\n", date_->year, date_->month, date_->day);
}

static void increment_year(void* date_vp) {
    struct Date* date = date_vp;
    date->year++;    
}

static void date_example(void) {
  {
    struct GrowableArray* a = create_array(date_cmp, free, duplicate_date);
    assert(a->size == 0);
    free_array(a);
  }
  
  {
    struct GrowableArray* a = create_array(date_cmp, free, duplicate_date);
    append(a, create_date(2017, 6, 2));
    append(a, create_date(2016, 6, 2));
    append(a, create_date(2021, 3, 2));
    append(a, create_date(2020, 4, 2));
    append(a, create_date(2019, 4, 2));
    
    struct GrowableArray* covid_years = filter(a, is_covid_year);
    struct Date* covid_date = covid_years->data[0];
    print_date(covid_date);
    //assert(covid_date->year == 2021);
    free_array(a);
    a = NULL;
    
    free(covid_date);
  }

  {
    struct GrowableArray* a = create_array(date_cmp, free, duplicate_date);
    append(a, create_date(2019, 6, 2));
    append(a, create_date(2016, 6, 2));
    append(a, create_date(2020, 3, 2));
    append(a, create_date(2020, 4, 2));
    append(a, create_date(2019, 4, 2));
    
    struct Date* max_date = max_element(a);
    assert(max_date->year == 2020);
    assert(max_date->month == 4);
    assert(max_date->day == 2);

    struct Date* min_date = min_element(a);
    assert(min_date->year == 2016);

    struct Date* last_date = pop(a);
    free(last_date);
    free_array(a);
    a = NULL;
  }
  
  {
    struct GrowableArray* a = create_array(date_cmp, free, duplicate_date);
    append(a, create_date(2019, 6, 2));
    append(a, create_date(2016, 6, 2));
    append(a, create_date(2020, 3, 2));
    
    for_each(a, increment_year);
    struct Date* increment_test = a->data[0];
    assert(increment_test->year == 2020);
    increment_test = a->data[1];
    assert(increment_test->year == 2017);
    increment_test = a->data[2];
    assert(increment_test->year == 2021);
    free_array(a);
  }
  
  {
    struct GrowableArray* a = create_array(date_cmp, free, duplicate_date);
    append(a, create_date(2019, 6, 2));
    append(a, create_date(2016, 6, 2));
    append(a, create_date(2020, 3, 2));
    append(a, create_date(2018, 4, 2));
    append(a, create_date(2026, 4, 2));
    
    insert(a, create_date(2021, 1, 11), 3);
    assert(a->size == 6);
    struct Date* insert_test = a->data[5];
    assert(insert_test->year == 2026);
    insert_test = a->data[4];
    assert(insert_test->year == 2018);
    insert_test = a->data[3];
    assert(insert_test->year == 2021);
    insert_test = a->data[2];
    assert(insert_test->year == 2020);
    insert_test = a->data[1];
    assert(insert_test->year == 2016);
    insert_test = a->data[0];
    assert(insert_test->year == 2019);
    free_array(a);
  }

  {
    struct GrowableArray* a = create_array(date_cmp, free, duplicate_date);
    append(a, create_date(2019, 6, 2));
    append(a, create_date(2016, 6, 2));

    struct GrowableArray* copy = duplicate(a);
    assert(copy->size == 2);

    struct Date* max_date = max_element(copy);
    assert(max_date->year == 2019);

    free_array(copy);
    free_array(a);
  }

  {
    struct GrowableArray* a = create_array(date_cmp, free, duplicate_date);
    append(a, create_date(2019, 6, 2));
    append(a, create_date(2016, 6, 2));

    struct Date* needle = create_date(2019, 6, 2);
    assert(contains(a, needle));

    needle->day = 20;
    assert(!contains(a, needle));

    free(needle);
    free_array(a);
  }

}

static int dummy_array_compare(const void* lhs, const void* rhs) {
  // silencing the unused parameter warnings, normally you'd want to actually
  // use your arguments
  (void)lhs;
  (void)rhs;
  return 0;
}

static void free_array_wrapper(void* array) {
  free_array(array);
}

static void* duplicate_array_wrapper(const void* array) {
  return duplicate(array);
}

static void nested_example(void) {
  struct GrowableArray* date_array1 = create_array(date_cmp, free, duplicate_date);
  struct GrowableArray* date_array2 = create_array(date_cmp, free, duplicate_date);

  struct GrowableArray* array_of_arrays =
    create_array(dummy_array_compare, free_array_wrapper, duplicate_array_wrapper);
  append(array_of_arrays, date_array1);
  append(array_of_arrays, date_array2);

  free_array(array_of_arrays);
}


int main(void) {
  date_example();
  nested_example();
}
