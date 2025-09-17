#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int is_all_digits(const char *s) {
    if (!*s) return 0;
    for (const char *p = s; *p; ++p) {
        if (!isdigit((unsigned char)*p)) return 0;
    }
    return 1;
}

static int luhn_valid(const char *s) {
    int sum = 0;
    int parity = 0;
    for (int i = (int)strlen(s) - 1; i >= 0; --i) {
        int d = s[i] - '0';
        if (parity) {
            int x = d * 2;
            sum += x / 10 + x % 10; // 等价于 >=10 时拆成两位数相加
        } else {
            sum += d;
        }
        parity ^= 1;
    }
    return sum % 10 == 0;
}

int main(void)
{
    char buf[64];

    printf("Number: ");
    if (!fgets(buf, sizeof buf, stdin))
    {
        puts("INVALID");
        return 0;
    }

    size_t n = strlen(buf);
    if (n && buf[n - 1] == '\n')
    {
        buf[--n] = '\0';
    }

    // 只接受纯数字输入
    if (!is_all_digits(buf)) {
        puts("INVALID");
        return 0;
    }

    int len = (int)strlen(buf);
    int first1 = buf[0] - '0';
    int first2 = (len >= 2) ? (buf[0] - '0') * 10 + (buf[1] - '0') : -1;

    if (!luhn_valid(buf)) {
        puts("INVALID");
        return 0;
    }

    // 品牌判定：长度 + 前缀
    // AMEX: 15位，以34或37开头
    // MASTERCARD: 16位，以51–55开头
    // VISA: 13或16位，以4开头
    // （与题面一致；不考虑万事达2系列） 
    if (len == 15 && (first2 == 34 || first2 == 37)) {
        puts("AMEX");
    } else if (len == 16 && (first2 >= 51 && first2 <= 55)) {
        puts("MASTERCARD");
    } else if ((len == 13 || len == 16) && first1 == 4) {
        puts("VISA");
    } else {
        puts("INVALID");
    }
    return 0;
}
