def get_height():
    while True:
        try:
            h = int(input("Height: ").strip())
            if 1 <= h <= 8:
                return h
        except ValueError:
            pass  # 非整数时继续重试

def main():
    h = get_height()
    for i in range(h):
        # 左侧空格 + 右侧砖块（无行尾多余空格）
        print(" " * (h - i - 1) + "#" * (i + 1))
    
main()
