# With this code, each store will have sales_rate column.
# sales_rate column shows us how much the store sells products each day.
# So, number of pallettes will be determined by that number.

import random
import pandas as pd

def generate_random_sales_rates(num_sales_rates):
    # Belirli olasılıklara sahip sayıları oluşturmak için
    possible_values = list(range(1, 7))  # 1 ila 6 arası sayılar
    probabilities = [0.05, 0.20, 0.30, 0.25, 0.15, 0.05]  # Örnek olasılık dağılımı

    sales_rates = random.choices(possible_values, probabilities, k=num_sales_rates)
    return sales_rates

def main():
    num_sales_rates = 79  # Mağaza sayısına uygun bir değer belirleyin
    sales_rates = generate_random_sales_rates(num_sales_rates)
    
    stores = pd.read_csv("./stores_with_coordinates.csv")
    store_id = stores["store_id"]
    
    sales_rates_df = pd.DataFrame({
        "store_id": store_id,
        "sales_rate": sales_rates
    })
    
    sales_rates_df.to_csv("sales_rates.csv", index=False)

if __name__ == "__main__":
    main()
