import os

extensions = (".c", ".cl", ".h") # Filter by extension

def count_total_lines(directory):
    total = 0
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(extensions): 
                full_path = os.path.join(root, file)
                with open(full_path, 'r', encoding='utf-8', errors='ignore') as f:
                    total += sum(1 for _ in f)
    return total

dir_path = "/home/botsz/code/ghost_at_particle_in_cell/0_src/"
print(f"Total project lines: {count_total_lines(dir_path)}")