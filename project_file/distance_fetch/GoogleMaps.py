import time
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

class GoogleMapsHandler:
    def __init__(self):
        self.chrome_options = webdriver.ChromeOptions()
        self.chrome_options.add_argument("--headless")
        self.chrome_options.add_argument("--disable-gpu")
        self.driver = webdriver.Chrome(options=self.chrome_options)
        self.wait = WebDriverWait(self.driver, 3)

    def fetch_label(self, value):
        return self.wait.until(EC.visibility_of_element_located((By.XPATH, value)))
    
    def send_location(self, value, latitude, longitude):
        value.send_keys(str(latitude) + "," + str(longitude))
    
    def fetch_duration_and_distance(self, latitude1, longitude1, latitude2, longitude2):
        url = f"https://www.google.com/maps/dir/{latitude1},{longitude1}/{latitude2},{longitude2}/data=!4m2!4m1!3e0?entry=ttu"
        self.driver.get(url)
        try:
            duration = self.wait.until(EC.visibility_of_element_located((By.XPATH, "/html/body/div[2]/div[3]/div[8]/div[9]/div/div/div[1]/div[2]/div/div[1]/div/div/div[4]/div[1]/div[1]/div/div[1]/div[1]")))
            distance = self.wait.until(EC.visibility_of_element_located((By.XPATH, "/html/body/div[2]/div[3]/div[8]/div[9]/div/div/div[1]/div[2]/div/div[1]/div/div/div[4]/div[1]/div[1]/div/div[1]/div[2]/div")))
        except Exception as e:
            print(e)
            return None, None
        return duration.text, distance.text
    
    def close_driver(self):
        self.driver.quit()
