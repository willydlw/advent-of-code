ranges, numbers = open(0).read().split('\n\n')

ranges = [list(map(int, r.split("-"))) for r in ranges.splitlines()]
numbers = list(map(int, numbers.splitlines()))

#print(ranges)
#print(numbers)

count = 0

for number in numbers:
    for low, high in ranges:
        if low <= number <= high:
            count += 1
            break 

print(count)