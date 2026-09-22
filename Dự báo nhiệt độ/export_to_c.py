import joblib
import emlearn
import shutil
import os

TARGETS    = ["temperature", "humidity", "log_light"]
SKETCH_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                          "..", "SmartGreenhouse")

for t in TARGETS:
    model = joblib.load(f"model_{t}.joblib")
    # dtype='float' -> giữ nguyên giá trị thực, không cần scale tay,
    # tránh mất thông tin ở các đặc trưng nhỏ như hour_sin/hour_cos
    cmodel = emlearn.convert(model, method="inline", dtype="float")
    filename = f"forecast_{t}.h"
    cmodel.save(file=filename, name=f"forecast_{t}")

    # Thêm include guard để tránh lỗi khi #include nhiều lần
    guard = f"FORECAST_{t.upper()}_H"
    with open(filename, "r") as f:
        content = f.read()
    with open(filename, "w") as f:
        f.write(f"#ifndef {guard}\n#define {guard}\n\n")
        f.write(content)
        f.write(f"\n#endif // {guard}\n")

    # Copy vào thư mục sketch để Arduino IDE tìm thấy khi build
    dest = os.path.join(SKETCH_DIR, filename)
    shutil.copy2(filename, dest)
    print(f"Đã xuất {filename} (dtype=float, guard: {guard}) → {dest}")