// Miller-Rabin Primality Test

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int find_greatest_common_divisor(int first_number, int second_number)
{
    return second_number ? find_greatest_common_divisor(second_number, first_number % second_number) : first_number;
}

int draw_random_integer(int exclusive_lower_bound, int exclusive_upper_bound)
{
    return rand() % (exclusive_upper_bound - exclusive_lower_bound + 1) + exclusive_lower_bound;
}

int exponentiate(int base, int index)
{
    if (base == 0) return 0;
    if (index == 0) return 1;
    if (index == 1) return base;
    
    int power = 1;
    
    while (index)
    {
        power *= power * exponentiate(base, index & 1);
        index >>= 1;
    }
    
    return power;
}

int exponentiate_modularly(int base, int index, int modulus)
{
    if (base == 0) return 0;
    if (index == 0) return 1;
    
    if (base > modulus) base %= modulus;
    if (index == 1) return base;
    
    int residue = 1;
    
    while (index)
    {
        if (index & 1) residue = (residue * base) % modulus;
        
        base = (base * base) % modulus;
        index >>= 1;
    }
    
    return residue;
}

int test_primality(int prime_candidate, int rounds)
{
    if (~prime_candidate & 1 || prime_candidate < 2) return 0;
    if (prime_candidate == 2) return 1;
    
    int greatest_power_of_two_factor_of_prime_candidate_less_one = 1;
    int prime_candidate_less_one = prime_candidate - 1;
    
    while (prime_candidate_less_one % exponentiate(2, greatest_power_of_two_factor_of_prime_candidate_less_one) == 0) 
        greatest_power_of_two_factor_of_prime_candidate_less_one++;
        
    greatest_power_of_two_factor_of_prime_candidate_less_one--;
    
    int multiplier = prime_candidate_less_one / exponentiate(2, greatest_power_of_two_factor_of_prime_candidate_less_one);
    
    for (int round = 1; round < rounds; round++)
    {
        // draw random base
        int base = draw_random_integer(1, prime_candidate_less_one);
        int greatest_common_divisor = find_greatest_common_divisor(base, prime_candidate);
        
        if (greatest_common_divisor > 1 && greatest_common_divisor < prime_candidate) return 0;
        
        base = exponentiate_modularly(base, multiplier, prime_candidate);
        
        if (base != 1 && base != prime_candidate_less_one)
        {
            for (int index = 1; index < greatest_power_of_two_factor_of_prime_candidate_less_one && base != prime_candidate_less_one; index++)
            {
                base = exponentiate_modularly(base, 2, prime_candidate);
                
                if (base == 1) return 0;
            }
            
            if (base != prime_candidate_less_one) return 0;
        }
    }
    
    return 1;
}

int main(int argument_count, char *arguments[])
{
    srand(time(NULL));
    
    int prime_candidate;
    
    if (argument_count > 1)
    {
        for (int argument = 1; argument < argument_count; argument++)
        {
            prime_candidate = atoi(arguments[argument]);
            
            if (test_primality(prime_candidate, 12)) printf("%d is a prime number.", prime_candidate);
            else printf("%d is not a prime number.", prime_candidate);
            
            printf("\n\n");
        }
    }
    
    for (;;)
    {
        printf("Enter a candidate integer to test for primality: ");
        
        // integer input validation
        // https://jackstromberg.com/2013/02/how-to-validate-numeric-integer-input-in-c/
        
        int input, status, buffer;
        
        status = scanf("%d", & input);
        
        while (status != 1)
        {
              while ((buffer = getchar()) != EOF && buffer != '\n');
              
              printf("Invalid input.");
              printf("\n\n");
              printf("Enter a candidate integer to test for primality: ");
              
              status = scanf("%d", & input);
        }
        
        prime_candidate = input;
        
        if (test_primality(prime_candidate, 12)) printf("%d is a prime number.", prime_candidate);
        else printf("%d is not a prime number.", prime_candidate);
        
        printf("\n\n");
    }
    
    return 0;
}
