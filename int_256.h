#ifndef INT_256_H
#define INT_256_H
#include <iostream>
#include <string>
// unsigned
class int_256
{
private:
    int digits[77]; // max val approx 2^256
public:
    void annul()
    {
        for(int i=0;i<77;i++)
        {
            digits[i]=0;
        }
    }

    std::string get()
    {
        std::string str;
        for(int i=0;i<77;i++)
        {
            str.push_back((char)(digits[i]+48));
        }
        bool is_zero=true;
        for(int i=0;i<77;i++)
        {
            if(str[i] != '0')
            {
                str.erase(str.begin(),str.begin()+i);
                is_zero=false;
                break;
            }
        }
        if(is_zero)
        {
            str="0";
        }

        return str;
    }

    void operator = (std::string str)
    {
        annul();
        if(str.length()>77)
        {
            std::cerr << "int_256 : out of size\n";
            throw *this;
        }
        if(str.find_first_not_of("1234567890") != std::string::npos)
        {

            std::cerr << "int_256 : non-numeral digit\n";
            throw *this;
        }

        int q = str.size();
        for(int i=76;i>76-q;i--)
        {
            digits[i]=(int)str[str.size()-1] - 48;
            str.erase(str.end()-1);
        }
    }

    void operator = (int_256 const i256)
    {
        for(int i=0;i<77;i++)
        {
            digits[i]=i256.digits[i];
        }
    }

    int_256 operator+ (int_256 const second)
    {
        int_256 sum;
        int carry = 0;
        for(int i=76;i>=0;i--)
        {   sum.digits[i]=carry;
            carry = (digits[i]+second.digits[i])/10;
            sum.digits[i] += digits[i]+second.digits[i]-10*carry;
        }
        if(sum.digits[0]>9)
        {
           std::cerr << "int_256 : out of size\n";
           throw sum;
        }
        return sum;
    }

    bool operator == (int_256 const second)
    {
        for(int i=0;i<77;i++)
        {
            if(digits[i]!=second.digits[i])
            {
                return false;
            }
        }
        return true;
    }

    bool operator > (int_256 const second)
    {
        for(int i=0;i<77;i++)
        {
            if(digits[i]>second.digits[i])
            {
                return true;
            }
            else if(digits[i]<second.digits[i])
            {
                return false;
            }
        }
        return false;
    }

    bool operator < (int_256 const second)
    {
        for(int i=0;i<77;i++)
        {
            if(digits[i]<second.digits[i])
            {
                return true;
            }
            else if(digits[i]>second.digits[i])
            {
                return false;
            }
        }
        return false;
    }

    bool operator <= (int_256 const second)
    {
        for(int i=0;i<77;i++)
        {
            if(digits[i]>second.digits[i])
            {
                return false;
            }
            else if(digits[i]<second.digits[i])
            {
                return true;
            }
        }
        return true;
    }

    bool operator >= (int_256 const second)
    {
        for(int i=0;i<77;i++)
        {
            if(digits[i]<second.digits[i])
            {
                return false;
            }
            else if(digits[i]>second.digits[i])
            {
                return true;
            }
        }
        return true;
    }

    int_256 operator - (int_256 const second)
    {
            if(*this < second)
            {
                std::cerr << "int_256 : negative value\n";
                throw *this;
            }
            int_256 difference;
            difference.annul();
            for(int i=76;i>=0;i--)
            {
                difference.digits[i]+=digits[i]-second.digits[i];
                if(difference.digits[i]<0)
                {
                    difference.digits[i]+=10;
                    difference.digits[i-1]-=1;
                }
            }
            return difference;
    }

    int_256 operator * (int const digit)
    {
        int_256 product;
        product.annul();
        for(int i=76;i>=0;i--)
        {
            product.digits[i] += digits[i]*digit;
            if(i==0 && product.digits[i]>9)
            {
                std::cerr << "int_256 : out of size\n";
                throw *this;
            }
            product.digits[i-1] += product.digits[i]/10;
            product.digits[i] = product.digits[i]%10;
        }
        return product;
    }

    void operator += (int_256 const toadd)
    {
        *this = *this + toadd;
    }

    int_256 operator * (int_256 const second)
    {
        int_256 product;
        product.annul();
        for(int i=76;i>=0;i--)
        {
               product += doTenthPow( (*this) * second.digits[i] , 76-i);
        }
        if(product.digits[0]>9)
        {
            std::cerr << "int_256 : out of size\n";
            throw *this;
        }
        return product;
    }

    int_256 doTenthPow(int_256 number,int tenthPower) // returns number*10^tenthPower
    {
        int_256 tt;
        tt.annul();
        if(number == tt)
        {
            return tt;
        }
        for(int i=tenthPower;i<77;i++)
        {
            number.digits[i-tenthPower]=number.digits[i];
        }
        return number;
    }

    int_256 power(int_256 exponent, int power)
    {
        int_256 result;
        result = exponent;
        for(int i=1;i<power;i++)
        {
            result = result * exponent;
        }
    }








    int_256(){}
};

#endif // INT_256_H
