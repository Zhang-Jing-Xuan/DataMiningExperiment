from collections import defaultdict
from itertools import count
from pathlib import Path
from typing import Any, List, Union

PATH = Path(__file__).parent.absolute()


def 载入数据(filename: Union[Path, str]):
    with open(PATH / filename) as f:
        for i in f:
            yield set(map(int, i.split()))


txt = {
    1: "T1014D1K.dat",
    10: "T1014D10K.dat",
    50: "T1014D50K.dat",
    100: "T1014D50K.dat",
    "t": "test.txt",
}
数据集id, 最小支持度 = (
    [(1, 6), (1, 8), (1, 10)] + [(10, 60), (10, 80), (10, 100)] + [(50, 500)]
)[1]
数据 = list(载入数据(txt[数据集id]))
所有元素 = set(frozenset([i]) for j in 数据 for i in j)
# 最小支持度 = 3
# 测试单次
def 迭代(迭代前: list[Any]):
    结果 = set()
    length = len(迭代前[0])
    for i in range(len(迭代前) - 1):
        for j in range(i + 1, len(迭代前)):
            t = 迭代前[i] | 迭代前[j]
            if len(t) == length + 1:
                结果.add(t)
    return 结果


with open(PATH / f"{数据集id}+{最小支持度}", mode="w") as f:
    for 迭代次数 in count(1):
        计数 = defaultdict(int)
        for i in 所有元素:
            for j in 数据:
                if j.issuperset(i):
                    计数[i] += 1
        计数 = defaultdict(int, {k: v for k, v in 计数.items() if v >= 最小支持度})
        迭代前 = list(计数.keys())
        if len(迭代前) == 0:
            break
        for i in 迭代前:
            print(*i, file=f)
        print(file=f)
        所有元素 = 迭代(迭代前)

"""
1K 6
22:17:03
22:17:32
10K
22:22:19
22:23:10
1K 10
16:07:10
16:07:18
1K 8
15:59:26
15:59:37
"""