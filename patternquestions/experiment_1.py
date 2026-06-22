# experiment_1.py - Vulnerable Login
import mysql.connector
from db_config import get_connection, close_connection

def experiment_1():
    print("\n" + "="*70)
    print("EXPERIMENT 1: VULNERABLE LOGIN (SQL Injection)")
    print("="*70)
    print("\n⚠️  This is VULNERABLE code - DO NOT use in production!")
    print("\n📝 Test with normal credentials AND with: ' OR '1'='1' --")
    print("="*70)
    
    email = input("\nEnter email: ")
    password = input("Enter password: ")
    
    conn = get_connection()
    cursor = conn.cursor()
    
    # 🔴 VULNERABLE: String concatenation
    query = f"SELECT * FROM customers WHERE email = '{email}' AND password = '{password}'"
    
    print(f"\n[!] Executing Query: {query}")
    print("[!] This query is VULNERABLE to SQL Injection!\n")
    
    try:
        cursor.execute(query)
        result = cursor.fetchall()
        
        if result:
            print(f"\n✅ LOGIN SUCCESSFUL!")
            print(f"👤 Welcome {result[0][1]}")
            print(f"📧 Email: {result[0][2]}")
            print("\n⚠️  VULNERABLE: SQL Injection bypassed login!")
        else:
            print("\n❌ Login failed")
            
    except Exception as e:
        print(f"\n❌ Error: {e}")
    
    close_connection(conn, cursor)

if __name__ == "__main__":
    experiment_1()