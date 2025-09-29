import csv
import sys

def main():

    # 检查命令行用法：需要两个参数：数据库 CSV 与 DNA 序列 TXT
    if len(sys.argv) != 3:
        print("Usage: python dna.py database.csv sequence.txt")
        sys.exit(1)

    db_path = sys.argv[1]
    seq_path = sys.argv[2]

    db_path = sys.argv[1]
    seq_path = sys.argv[2]

    # 将数据库文件读入到变量
    with open(db_path, newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        fieldnames = reader.fieldnames or []
        if len(fieldnames) < 2 or fieldnames[0] != "name":
            print("Invalid database header")
            sys.exit(1)

        strs = fieldnames[1:]
        people = []
        for row in reader:
            person = {"name": row["name"]}
            for s in strs:
                person[s] = int(row[s])  # 将计数转为整数便于比较
            people.append(person)

    # 将 DNA 序列文件读入到变量
    with open(seq_path, "r", encoding="utf-8") as f:
        sequence = f.read().strip()

    # 在 DNA 序列中为每个 STR 计算“最长连续重复次数”
    profile = {s: longest_match(sequence, s) for s in strs}

    # 在数据库中查找匹配的个人资料：所有 STR 计数完全一致即为匹配
    for person in people:
        if all(person[s] == profile[s] for s in strs):
            print(person["name"])
            return

    # 若未找到匹配
    print("No match")
    return


def longest_match(sequence, subsequence):
    """返回 subsequence 在 sequence 中出现的最长“连续重复”次数。"""

    # 初始化变量
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # 逐个字符作为起点，寻找从该处开始的最多连续重复次数
    for i in range(sequence_length):

        # 当前起点的连续次数计数器
        count = 0

        # 在 sequence 中取“子串窗口”与 subsequence 比较
        # 若匹配，则将窗口向右移动一个 subsequence 的长度继续比较
        # 直到不再连续匹配为止
        while True:

            # 计算当前窗口的起止位置
            start = i + count * subsequence_length
            end = start + subsequence_length

            # 若窗口子串与 subsequence 匹配，累计次数
            if sequence[start:end] == subsequence:
                count += 1
            
            # 若不匹配，跳出循环
            else:
                break
        
        # 更新目前为止的最长连续次数
        longest_run = max(longest_run, count)

    # 扫描完成后，返回最长连续次数
    return longest_run


main()
