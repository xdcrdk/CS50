def get_height():
    while True:
        try:
            h = int(input("Height: ").strip())
            if 1 <= h <= 8:
                return h
        except ValueError:
            pass  # 非整数时继续询问

def main():
    h = get_height()
    for i in range(1, h + 1):
        # 左金字塔左侧空格 + 左金字塔砖块 + 两个空格 + 右金字塔砖块
        print(" " * (h - i) + "#" * i + "  " + "#" * i)

main()