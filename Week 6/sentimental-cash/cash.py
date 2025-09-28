def get_amount():
    while True:
        try:
            s = input("Change owed: ").strip()
            amt = float(s)
            if amt >= 0:
                return amt
        except ValueError:
            pass  # 非数字/空输入会重试

def main():
    dollars = get_amount()
    cents = round(dollars * 100)  # 按最接近的分取整
    coins = 0
    for coin in (25, 10, 5, 1):
        q, cents = divmod(cents, coin)
        coins += q
    print(coins)  # 只输出整数和换行

main()
