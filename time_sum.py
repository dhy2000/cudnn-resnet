# Usage:
#    ./resnet > stdout.log
#    python3 -u time_sum.py 

with open('stdout.log', "r") as fp:
    lines = fp.readlines()

def try_float(s: str) -> float:
    try:
        return float(s)
    except ValueError as _:
        return 0

# print(sum([try_float(x) for x in txt.replace(',', ' ').replace('\n', ' ').strip().split(' ')]))
print(sum([try_float(x) for x in lines]))