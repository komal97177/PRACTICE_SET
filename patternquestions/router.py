import pandas as pd
from collections import Counter

# Read log file
log_file = "log_router.txt"

data = []

with open(log_file, "r") as file:
    for line in file:
        parts = line.strip().split()

        if len(parts) >= 5:
            date = parts[0]
            time = parts[1]
            event = parts[2]
            mac = parts[3]
            device = " ".join(parts[4:])

            data.append([date, time, event, mac, device])

# Create DataFrame
df = pd.DataFrame(
    data,
    columns=["Date", "Time", "Event", "MAC_ID", "Device"]
)

print("\n=== Original Dataset ===")
print(df)

# --------------------------------------------------
# 1. Repetitive vs Unique Log Entries
# --------------------------------------------------

log_counts = Counter(df.astype(str).agg(' '.join, axis=1))

repetitive_logs = {k: v for k, v in log_counts.items() if v > 1}
unique_logs = {k: v for k, v in log_counts.items() if v == 1}

print("\n=== Repetitive Logs ===")
for log, count in repetitive_logs.items():
    print(count, "times:", log)

print("\nTotal Unique Logs:", len(unique_logs))

# --------------------------------------------------
# 2. MAC IDs / Devices Connected
# --------------------------------------------------

print("\n=== Connected Devices ===")
devices = df[["MAC_ID", "Device"]].drop_duplicates()
print(devices)

# --------------------------------------------------
# 3. Connection/Login Count per MAC
# --------------------------------------------------

connection_count = df[df["Event"] == "CONNECT"].groupby(
    "MAC_ID"
).size()

print("\n=== Connection Count per MAC ===")
print(connection_count)

# --------------------------------------------------
# 4. Connection Duration
# --------------------------------------------------

df["Timestamp"] = pd.to_datetime(
    df["Date"] + " " + df["Time"]
)

durations = []

for mac in df["MAC_ID"].unique():

    mac_logs = df[df["MAC_ID"] == mac].sort_values(
        "Timestamp"
    )

    connect_time = None

    for _, row in mac_logs.iterrows():

        if row["Event"] == "CONNECT":
            connect_time = row["Timestamp"]

        elif row["Event"] == "DISCONNECT" and connect_time:

            duration = (
                row["Timestamp"] - connect_time
            ).total_seconds() / 60

            durations.append([
                mac,
                duration
            ])

            connect_time = None

duration_df = pd.DataFrame(
    durations,
    columns=["MAC_ID", "Duration_Minutes"]
)

print("\n=== Connection Duration ===")
print(duration_df)

# --------------------------------------------------
# 5. Data Cleaning
# --------------------------------------------------

# Remove duplicates
df.drop_duplicates(inplace=True)

# Remove null values
df.dropna(inplace=True)

# Standardize MAC IDs
df["MAC_ID"] = df["MAC_ID"].str.upper()

print("\n=== Cleaned Dataset ===")
print(df)

# --------------------------------------------------
# 6. Export Files
# --------------------------------------------------

csv_file = "router_logs_cleaned.csv"
xlsx_file = "router_logs_cleaned.xlsx"

df.to_csv(csv_file, index=False)

with pd.ExcelWriter(xlsx_file) as writer:
    df.to_excel(writer, sheet_name="Logs", index=False)
    duration_df.to_excel(
        writer,
        sheet_name="Connection_Duration",
        index=False
    )

print("\nCSV Exported:", csv_file)
print("Excel Exported:", xlsx_file)