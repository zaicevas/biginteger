#include "BigInteger.h"

bint bint_cpy(bint);
bint list_to_bint(List*, Sign);
bint do_division(bint, bint, bool);
Sign div_mult_sign(Sign, Sign);
mtable get_table(bint);
char* int_to_string(int);

bint bint_cpy(bint source) {
    return (bint) {list_cpy(source.list), source.sign};
}

bint list_to_bint(List *list, Sign sign) {
    bint bigint;
    bigint.list = list;
    bigint.sign = sign;
    return bigint;
}

bint string_to_bint(char *string) {
    bint bigint;
    if (string[0] == '0' || (string[0] == '-' && string[1] == '0')) {
        bigint.sign = positive;
        bigint.list = string_to_list("0");
    }
    else if (string[0] == '-') {
        bigint.sign = negative;
        bigint.list = string_to_list(string+1);
    }
    else {
        bigint.sign = positive;
        bigint.list = string_to_list(string);
    }
    return bigint;
}

char* bint_to_string(bint biginteger) {
    if (is_empty(*(biginteger.list))) {
        char *arr = malloc(2);
        arr[0] = '0';
        arr[1] = 0;
        return arr;
    }
    else if (biginteger.sign == positive)
        return list_to_string(reverse_list(biginteger.list), 0);
            else
                return list_to_string(reverse_list(biginteger.list), '-');
}

bint bint_add(bint a, bint b) {
    List *result = create_list(0), *shorter, *longer;
    Sign s;
    int min, max, i, carry=0;
    if (!a.list || !b.list)
        return string_to_bint("0");
    if (a.sign == positive && b.sign == positive)
        s = positive;
    else if (b.sign == negative && a.sign == negative)
        s = negative;
    else if (a.sign == negative) {          /* kai paduodam i sub, du minusai pavirsta pliusu, todel keiciam neigiamo sk. zenkla */
        a.sign = positive;
        return bint_sub(b, a);
    }
    else {                                  /* b.sign == negative */
        b.sign = positive;
        return bint_sub(a, b);
    }

    if (a.list->count == b.list ->count) {
        min = a.list->count;
        max = min;
        shorter = a.list;
        longer = b.list;
    }
    else {
        shorter = get_shorter(a.list, b.list, &min);
        longer = get_longer(a.list, b.list, &max);
    }
    for (i = 0; i < max; i++) {
        int shorter_data = 0, longer_data = get_by_index(longer, i)->data, new_data;
        if (i < min)
            shorter_data = get_by_index(shorter, i)->data;
        new_data = shorter_data + longer_data + carry;
        carry = 0;
        while (new_data > 9)
        {
            new_data -= 10;
            carry++;
        }
        push_back(result, (char)new_data);
    }
    if (carry > 0)
        push_back(result, (char)carry);
    return list_to_bint(result, s);
}

bint bint_sub(bint a, bint b) {
    int i=0;
    bint result = {create_list(0), 0};
    if (!a.list || !b.list)
        return string_to_bint("0");
    if (a.sign != b.sign) {                                        /* a-(-b) = a+b arba -a-b = -(a+b) */
        b.sign = a.sign ? negative : positive;
        return bint_add(a,b);
    }
    if (bint_equal(a, b))
        return string_to_bint("0");

    /* if we got there and |a| > |b|, we do a regular substraction
    and the sign won't change (both numbers will have the same) */

    if (bint_greater_than((bint) {a.list, positive}, (bint) {b.list, positive})) {
        bool carry = false;
        Node *head1 = a.list->HEAD, *head2 = b.list->HEAD, *curr;
        List *reversed;
        while (head1) {
            int tmp;
            tmp = head2 ? head1->data - head2->data : head1->data;
            if (carry) {
                tmp--;
                carry = false;
            }
            if (tmp < 0) {
                carry = true;
                tmp += 10;
            }
            push_back(result.list, tmp);
            head1 = head1->next;
            if (head2)
                head2 = head2->next;
            i++;
        }
        for (; i<a.list->count; i++) {
            push_back(result.list, head1->data);
            head1 = head1->next;
        }
        /* let's check if we have zeros at the left of a number */
        reversed = reverse_list(result.list);
        curr = reversed->HEAD;
        while (curr && curr->data == 0) {
            curr = curr->next;
            pop_front(reversed);
        }
        result.list = reverse_list(result.list);
        result.sign = a.sign;
    }
    else {                                /* if a < b, then we calculate by this principle: a-b = -(b-a) */
        result = bint_sub(b, a);
        result.sign = !a.sign;
    }
    return result;
}

bint bint_mul(bint a, bint b) {
    if (!a.list || !b.list)
        return string_to_bint("0");
    if (is_zero(a) || is_zero(b))
        return string_to_bint("0");
    Sign s = div_mult_sign(a.sign, b.sign);
    mtable t = get_table(a);
    bint result = {create_list(0), s};
    int i;

    for (i = 0; i < b.list->count; i++) {
        bint value = bint_cpy(t.values[(unsigned) get_by_index(b.list, i)->data]);
        int j = i;
        value.sign = s;
        while (j > 0) {
            push_front(value.list, 0);
            j--;
        }
        bint freeable = result;
        result = bint_add(result, value);
        free_bint(freeable);
        free_bint(value);
    }
    for (int i=0; i<10; i++)
        free_bint(t.values[i]);
    return result;
}

bint bint_div(bint a, bint b) {
    if (!a.list || !b.list)
        return string_to_bint("0");
    if (is_zero(b))
        return int_to_bint(0);
    if (bint_greater_than((bint) {b.list, positive}, (bint) {a.list, positive}))
        return int_to_bint(0);
    return do_division(a, b, 1);
}

bint bint_mod(bint a, bint b) {
    if (!a.list || !b.list)
        return string_to_bint("0");
    if (is_zero(b))
        return int_to_bint(0);
    return do_division(a, b, 0);
}

bint do_division(bint a, bint b, bool div) {
/* An optimized division: we multiply B by 10 until it is smaller than A,
then we do substractions and, with additional calculations, get the final answer */
    Sign bs = b.sign;
    int i;
    unsigned difference=1;
    bint tmp_a = {list_cpy(a.list), positive}, sub_final, ten_laipsnis, ten,
    freeable, result, tmp2, j, vienas = int_to_bint(1);
    if (is_zero(a))
        return string_to_bint("0");
    tmp_a.sign = positive;
    b.sign = positive;
    ten_laipsnis = int_to_bint(1);

    if (a.list->count > b.list->count)
        difference = a.list->count - b.list->count;
    ten = int_to_bint(10);
    // we've found biggest 10 index, such that 10^index * b < a
    for (i=0; i<difference-1; i++) {
        freeable = ten_laipsnis;
        ten_laipsnis = bint_mul(ten_laipsnis, ten);
        free_bint(freeable);
    }

    tmp2 = bint_mul(ten_laipsnis, b);
    freeable = tmp2;
    tmp2 = bint_mul(tmp2, ten);
    free_bint(freeable);
    if (bint_lesser_than(tmp2, a)) {
        freeable = ten_laipsnis;
        ten_laipsnis = bint_mul(ten_laipsnis, ten);
        free_bint(freeable);
    }
    free_bint(tmp2);
    sub_final = bint_mul(ten_laipsnis, b);
    j = int_to_bint(0);
    // do substractions with sub_final
    while (bint_greater_than(tmp_a, sub_final) || bint_equal(tmp_a, sub_final)) {
        freeable = tmp_a;
        tmp_a = bint_sub(tmp_a, sub_final);
        free_bint(freeable);
        freeable = j;
        j = bint_add(j, vienas);
        free_bint(freeable);
    }
    // multiply temporary div answer by 10^index and add remainder, which is divided by b
    result = bint_mul(j, ten_laipsnis);
    freeable = result;
    bint freeable_2 = bint_div(tmp_a, b);
    result = bint_add(freeable_2, result);
    free_bint(freeable_2);
    free_bint(freeable);
    free_bint(ten);
    free_bint(vienas);
    free_bint(j);
    free_bint(sub_final);
    free_bint(ten_laipsnis);
    if (div) {
        free_bint(tmp_a);
        result.sign = a.sign == bs || is_zero(j) ? positive : negative;
        return result;
    }
        else {
            bint freeable2 = bint_div(tmp_a, b), freeable3 = bint_mul(b, freeable2);
            freeable = tmp_a;
            tmp_a = bint_sub(tmp_a, freeable3);
            free_bint(freeable);
            free_bint(freeable2);
            free_bint(freeable3);
            if ( (a.sign != bs && bs == positive && !is_zero(tmp_a)) || (a.sign == bs && bs == negative))
                tmp_a.sign = negative;
            free_bint(result);
            return tmp_a;
        }
}

bool is_zero(bint val) {
    Node *head;
    if (!val.list || is_empty(*(val.list)))
        return false;
    head = val.list->HEAD;
    while (head)
    {
        if (head->data != 0)
            return 0;
        head = head->next;
    }
    return 1;
}

Sign div_mult_sign(Sign a, Sign b) {
    return a == b ? positive : negative;
}

mtable get_table(bint val) {
    int i, j;
    mtable table;
    table.values[0] = list_to_bint(create_list(val.list->count), val.sign);
    table.values[1] = bint_cpy(val);
    for (i = 2; i < 10; i++)
    {
        table.values[i] = bint_cpy(val);
        for (j = 0; j < i - 1; j++)
        {
            bint freeable = table.values[i];
            table.values[i] = bint_add(table.values[i], val);
            free_bint(freeable);
        }
    }
    return table;
}

bool bint_equal(bint a, bint b) {
    Node *head1, *head2;
    if (!a.list || !b.list)
        return false;
    if (a.list->count == 0 || b.list->count == 0) {
        check_error(LIST_EMPTY_EXCEPTION);
        return false;
    }
    head1=a.list->HEAD, head2=b.list->HEAD;
    if (a.list->count != b.list->count)
        return false;
    while (head1 && head2) {
        if (head1->data != head2->data)
            return false;
        head1 = head1->next;
        head2 = head2->next;
    }
    return true;
}

bool bint_greater_than(bint a, bint b) {
    Node *head1=NULL, *head2=NULL;
    bool bresult = false;
    if (!a.list || !b.list || is_empty(*(a.list)) || is_empty(*(b.list)))
        return false;
    if (a.list->count == 0 || b.list->count == 0) {
        check_error(LIST_EMPTY_EXCEPTION);
        return false;
    }
    if (a.list->count < b.list->count)
        return false;
        else if (a.list->count > b.list->count)
            return true;
    a.list = reverse_list(a.list);
    b.list = reverse_list(b.list);
    head1 = a.list->HEAD;
    head2 = b.list->HEAD;
    while (head1 && head2) {
        if ( (head1->data > head2->data && a.sign == positive) ||
        (head1->data < head2->data && b.sign == negative) ) {
            bresult = true;
            break;
        }
            else if (head1->data != head2->data)
                break;
        head1 = head1->next;
        head2 = head2->next;
    }
    a.list = reverse_list(a.list);
    b.list = reverse_list(b.list);
    return bresult;
}

bool bint_lesser_than(bint a, bint b) {
    if (!a.list || !b.list || is_empty(*(a.list)) || is_empty(*(b.list)))
        return false;
    return !(bint_greater_than(a,b) || bint_equal(a, b));
}

char* int_to_string(int a) {
    int tmp = a, digits=0, i, iki;
    bool negative = a < 0;
    char *arr;
    if (a == 0) {
        arr = calloc(2, 1);
        arr[0] = '0';
        return arr;
    }
    while (tmp) {
        tmp /= 10;
        digits++;
    }
    tmp = a;
    arr = calloc(digits+1+negative, 1);
    i = digits-1, iki = 0;
    if (negative) {
        i++;
        iki++;
        arr[0] = '-';
    }
    for (; i>=iki; i--) {
        char final = tmp ;
        if (negative)
            final *= -1;
        arr[i] = (char) final + 0x30;
        tmp /= 10;
    }
    return arr;
}

void free_bint(bint a) {
    free_list(a.list);
}

bint int_to_bint(int a) {
    char *tmp = int_to_string(abs(a));
    bint result = {string_to_list(tmp), a < 0 ? negative : positive};
    free(tmp);
    return result;
}

void print_bint(bint a) {
    char *tmp = bint_to_string(a);
	printf("%s", tmp);
    free(tmp);
}
