def get_number() -> str:
    """反复提示直到输入只含数字（允许输入中含空格或连字符，会被去掉）"""
    while True:
        s = input("Number: ")
        # 去掉常见分隔符
        s = s.replace(" ", "").replace("-", "")
        if s.isdigit() and len(s) > 0:
            return s
        # 非数字则继续提示

def luhn_valid(num: str) -> bool:
    """Luhn 校验：从末位起每隔一位加倍，拆分相加，总和对 10 取模为 0 即有效"""
    total = 0
    double = False
    for ch in reversed(num):
        d = ord(ch) - 48  # 更快的 int(ch)
        if double:
            d *= 2
            # 加倍后如果是两位数，相当于各位数相加（等价于 d - 9）
            if d > 9:
                d -= 9
        total += d
        double = not double
    return total % 10 == 0

def card_type(num: str) -> str:
    n = len(num)
    start1 = num[0]
    start2 = num[:2]

    if not luhn_valid(num):
        return "INVALID"

    # American Express: 15 位，开头 34 或 37
    if n == 15 and start2 in ("34", "37"):
        return "AMEX"

    # MasterCard: 16 位，开头 51–55（按 CS50 题面）
    if n == 16 and start2 in ("51", "52", "53", "54", "55"):
        return "MASTERCARD"

    # Visa: 13 或 16 位，开头 4
    if start1 == "4" and n in (13, 16):
        return "VISA"

    return "INVALID"

def main():
    num = get_number()
    print(card_type(num))

main()