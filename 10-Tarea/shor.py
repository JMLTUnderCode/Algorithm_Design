import random
import math

# El algoritmo de Shor depende de la exponenciación modular
# Utilizamos la función integrada de Python para eficiencia y precisión
# pow(a, b, m) calcula (a^b) mod m
# math.gcd calcula el máximo común divisor

def find_order(x, N):
    """
    Encuentra el orden r de x módulo N.
    (Simula la función de búsqueda de periodo de QFT de forma clásica).
    """
    if math.gcd(x, N) != 1:
        return 0  # Caso trivial: ya encontramos un factor
    
    r = 1
    val = x
    while val != 1:
        val = pow(x, r, N)
        if val == 1:
            break
        r += 1
    return r

def shor_simulation(num_sim, N, k_max=10):
    
    print(f"--- Simulación {num_sim} del Algoritmo de Shor para N = {N} ---")
    
    for i in range(1, k_max + 1):
        # 1. Seleccionar una base aleatoria x
        x = random.randint(2, N - 1)
        
        # 2. Verificar la coprimalidad
        d_init = math.gcd(x, N)
        if d_init != 1:
            print(f"\nIteración {i}/{k_max}: Base x = {x}. GCD(x, N) = {d_init}.")
            if 1 < d_init < N:
                print(f"Éxito: ¡Factor no trivial encontrado en el paso inicial! Factor = {d_init}")
                return True, i
            # Si d_init = N, el test es inútil; si d_init=1, procedemos
        
        # 3. Encontrar el orden r (período)
        r = find_order(x, N)
        
        print(f"\nIteración {i}/{k_max}: Base x = {x}. Orden r = {r}.")

        # 4. Verificar condiciones de factorización: r debe ser par
        if r == 0 or r % 2 != 0:
            print(f"Resultado: Orden r={r} no es par/válido. Continuar.")
            continue
        
        # 5. Verificar condición de no-trivialidad de la raíz
        r_half = r // 2
        
        # Calculamos x^(r/2) mod N
        x_r_half_mod_N = pow(x, r_half, N)
        print(f"Verificación: x^(r/2) mod N = {x_r_half_mod_N}")

        if x_r_half_mod_N == N - 1:
            print(f"Resultado: x^(r/2) = N-1 ({N-1}). Raíz trivial. Continuar.")
            continue
        
        # 6. Calcular factores no triviales
        d1 = math.gcd(N, x_r_half_mod_N + 1)
        d2 = math.gcd(N, x_r_half_mod_N - 1)
        
        if (1 < d1 < N) or (1 < d2 < N):
            factor = d1 if 1 < d1 < N else d2
            print(f"Éxito: ¡Factor no trivial encontrado! {N} = {factor} * {N // factor}")
            return True, i
        else:
            print("Resultado: Factor trivial obtenido. Continuar.")
            
    print(f"\n--- Simulación Finalizada ---")
    return False, k_max

if __name__ == "__main__":
    for num_sim in range(1, 11):
        result, iterations = shor_simulation(num_sim, 35, 10)
        if result:
            print(f"El algoritmo encontró un factor no trivial en {iterations} iteraciones.\n")
        else:
            print(f"El algoritmo no encontró un factor no trivial después de {iterations} intentos.\n")