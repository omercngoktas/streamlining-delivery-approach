from random import randint

def generate_random_values(interval, numberOfRandomNumbers):
    list = []
    for i in range(numberOfRandomNumbers):
        list.append(randint(interval[0], interval[1]))
    return list

def main():
    list = generate_random_values([1,6], 30)

    for i in list:
        print(i)
if __name__ == "__main__":
    main()