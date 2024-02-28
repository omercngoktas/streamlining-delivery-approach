# This code generates unique id for given depots or stores.

import pandas as pd
import random
import string

def generate_store_id(length):
    characters = string.ascii_letters + string.digits
    store_id = ''.join(random.choice(characters) for _ in range(length))
    return store_id

def main(type="store"):
    id_df = pd.DataFrame(columns=["store_id"])
    path = "./depot.csv"
    main_df = pd.read_csv(path)

    if type == "store":    
        header = "MG-"
    elif type == "depot":
        header = "DP-"
        
    for i in main_df.index:
        store_id = generate_store_id(20).upper()
        id_df = id_df.append({"store_id": header + store_id}, ignore_index=True)

    result_df = pd.concat([id_df, main_df], axis=1)
    result_df.to_csv(path+"test")

if __name__ == "__main__":
    main(type="depot")
