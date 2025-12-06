import random

def random_number(min, max):
    return random.randint(min, max)

def BTest(a, n):
    if a <= 1 or a >= n - 1:
        return False
    t = n - 1
    s = 0
    while t % 2 == 0:
        t //= 2
        s += 1
    print(f"Valor de t: {t}")
    print(f"Valor de s: {s}")
    print(f"Descomposición N-1: 2^s * t -> 2^{s} * {t}")
    x = pow(a, t, n)
    print(f"Valor de x: x = a^t mod n -> {a}^{t} mod {n} = {x}")
    if x == 1 or x == n - 1:
        return True
    for _ in range(s - 1):
        x = pow(x, 2, n)
        print(f"MOD: x = x^2 mod n -> {x}^2 mod {n} = {x}")
        if x == n - 1:
            return True
    return False

def miller_rabin(n, k):
    if n <= 1 or n == 4:
        return False
    if n <= 3:
        return True
    for i in range(k):
        a = random_number(2, n - 2)
        print(f"\nIteración {i + 1}: base 'a' = {a}")
        if not BTest(a, n):
            return False
    return True

if __name__ == "__main__":
    numero = 181093918109391810939
    k = 10
    es_primo = miller_rabin(numero, k)
    if es_primo:
        print(f"\nN = {numero} es probablemente primo.")
    else:
        print(f"\nN = {numero} no es primo.")
