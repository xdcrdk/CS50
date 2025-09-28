def count_letters(text: str) -> int:
    return sum(1 for ch in text if ch.isalpha())

def count_words(text: str) -> int:
    # split() 会对任意空白分隔，并自动跳过多余空格/换行
    return len(text.split())

def count_sentences(text: str) -> int:
    return sum(1 for ch in text if ch in ".!?")

def main():
    text = input("Text: ")

    letters = count_letters(text)
    words = count_words(text) or 1  # 防御性写法，避免除以 0
    sentences = count_sentences(text)

    L = letters * 100 / words           # 每 100 个单词中的字母数
    S = sentences * 100 / words         # 每 100 个单词中的句子数
    index = 0.0588 * L - 0.296 * S - 15.8

    grade = round(index)

    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")

main()
