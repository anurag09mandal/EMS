
import csv

def convert_csv_to_cpp(csv_file):
    cpp_lines = []
    with open(csv_file, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            # Normalize keys and values
            reg_id = row.get('Reg ID', '').strip()
            data_name = row.get('Data Name', '').strip()
            log_type = row.get('Log Type', '').strip()
            log_file_name = row.get('Log File Name', '').strip().replace(" ", "_") + ".csv"

            # Convert Write Cycle to seconds
            write_cycle = row.get('Write Cycle', '').lower()
            write_cycle_seconds = 0
            if 'minute' in write_cycle:
                write_cycle_seconds = int(write_cycle.split()[0]) * 60
            elif 'hour' in write_cycle:
                write_cycle_seconds = int(write_cycle.split()[0]) * 3600

            # Convert Log Type Creation Time to seconds
            rotation_time = row.get('Log Type Creation Time', '').lower()
            rotation_seconds = 0
            if 'day' in rotation_time:
                rotation_seconds = int(rotation_time.split()[0]) * 86400
            elif 'hour' in rotation_time:
                rotation_seconds = int(rotation_time.split()[0]) * 3600

            # Convert File Size to bytes
            file_size = row.get('File Size', '').lower()
            size_bytes = 0
            if 'kb' in file_size:
                size_bytes = int(file_size.split()[0]) * 1024
            elif 'mb' in file_size:
                size_bytes = int(file_size.split()[0]) * 1024 * 1024
            elif file_size.isdigit():
                size_bytes = int(file_size)  # Assume bytes if no unit

            # Generate C++ mapping line
            cpp_lines.append(
                f'logMapping["{data_name}"] = LogConfig{{"{reg_id}", "{log_type}", "{log_file_name}", {write_cycle_seconds}, {rotation_seconds}, {size_bytes}}};'
            )

    return "\n".join(cpp_lines)

# Example usage:
print(convert_csv_to_cpp("log_config.csv"))
