#include <stdio.h>
#include <math.h>
#include <cs50.h>

int wrong_size_number(long long);
int aint_no_luhn(long long);
int extract_leading_digits(long long);
void print_card_from_digits(int);

int main(void)
{
    // get user input
    printf("Number: ");
    long long card = GetLongLong();
    
    // check if the input structure is invalid for a credit card
    if (wrong_size_number(card) || aint_no_luhn(card))
    {
        printf("INVALID\n");
    }
    else
    {
        // pull out the leading digits to determine the card type
        int leading_digits = extract_leading_digits(card);
        
        print_card_from_digits(leading_digits);
    }
    
    return 0;
}

int wrong_size_number(long long card)
{
    // we need a positive integer at least 1x10^13 and less than 1x10^17
    return card < pow(10, 12) || card >= pow(10, 16);
}

int aint_no_luhn(long long card)
{
    int last_digit;
    int even = 1;
    int accumulator = 0;
    for (; card > 0; card /= 10)
    {
        // grab the last digit and if an odd digit, * 2
        last_digit = (even)? card % 10 : (card % 10) * 2;
        
        // if the last digit is now >= 10, add the digits
        if (last_digit >= 10)
        {
            last_digit = last_digit % 10 + last_digit / 10;
        }
        
        // accumulate and then flip even
        accumulator += last_digit;
        even = 1 - even;
    }
    
    // test for luhn validity then return 1 for invalid ("aint no luhn")
    bool valid_cc = accumulator % 10 == 0;
    return !valid_cc;
}

int extract_leading_digits(long long card)
{
    // return out first 2 digits
    for(; card > 0; card /= 10)
    {
        if (card < 100)
        {
            return card;
        }
    }
    
    // failsafe should never be reached
    printf("Whoops!");
    return 0;
}

void print_card_from_digits(int digits)
{
    switch (digits)
    {
        case 34: // fall through
        case 37:
            printf("AMEX\n");
            break;
        case 51: // fall through
        case 52: // fall through
        case 53: // fall through
        case 54: // fall through
        case 55:
            printf("MASTERCARD\n");
            break;
        case 40: // fall through
        case 41: // fall through
        case 42: // fall through
        case 43: // fall through
        case 44: // fall through
        case 45: // fall through
        case 46: // fall through
        case 47: // fall through
        case 48: // fall through
        case 49:
            printf("VISA\n");
            break;
        default:
            printf("INVALID\n");
            break;
    }
}