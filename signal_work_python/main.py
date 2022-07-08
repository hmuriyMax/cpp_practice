import time


if __name__ == '__main__':
    try:
        print("Im doing some long long process...")
        while True:
            continue
    except KeyboardInterrupt:
        print("Fuck! I got KeyboardInterrupt! Finishing...")
        time.sleep(2)
