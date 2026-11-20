# Ghi chú bàn giao — tiếp tục viết báo cáo DATN

Thư mục này gom toàn bộ công cụ và nội dung đang dùng để sinh các chương báo cáo (Word).
Mục đích: chuyển máy (Windows → Linux) mà không mất ngữ cảnh. File trước đây nằm ở thư mục
scratchpad tạm của Claude Code nên không theo repo; nay đã đưa vào đây để đồng bộ qua git.

## Nội dung thư mục

| File | Vai trò |
|---|---|
| `build.ps1` | Sinh file .docx từ file nội dung gắn thẻ (dùng Word COM). **CHỈ CHẠY ĐƯỢC TRÊN WINDOWS.** |
| `figs.ps1` | Vẽ Hình 2.30 (pipeline) và 2.31 (biểu đồ dữ liệu cảm biến) bằng System.Drawing. **CHỈ WINDOWS.** |
| `labels.txt` | Nhãn text cho figs.ps1 |
| `content25.txt` | Nguồn Mục 2.5 "Xây dựng mô hình AI" (phiên bản mới nhất) |
| `content.txt` | Nguồn Chương 4 (kịch bản 1–3) |
| `content4.txt` | Nguồn Kịch bản 4 (độ bền LoRa) |
| `text.txt` | Bản nháp cũ (tham khảo) |
| `media/` | Các hình đã chèn: `hinh_2_27..31_*.png` |
| `transcript_conversation.jsonl.gz` | Toàn bộ lịch sử hội thoại (giải nén: `gunzip`) |

## File .docx kết quả (nằm ở thư mục gốc repo, không trong đây)

- `kiểm thử_hoàn chỉnh.docx` — Chương 4, mục 4.1–4.4
- `kịch bản 4_LoRa.docx` — mục 4.5
- `2.5_Xây dựng mô hình AI.docx` — bản cũ (đang mở trong Word lúc bàn giao)
- `2.5_Xây dựng mô hình AI_v2.docx` — **bản mới nhất** của Mục 2.5 (code đã rút gọn + tô màu cú pháp)

## Cách build lại .docx (Windows)

```powershell
# Mục 2.5:
powershell -File build.ps1 -ContentFile content25.txt -MediaDir media -OutFile "..\2.5_Xây dựng mô hình AI_v2.docx"
# Vẽ lại hình (nếu đổi dữ liệu): figs.ps1 -DataDir "..\Dự báo nhiệt độ\Data" -OutDir media
```
Lưu ý: đóng file trong Word trước khi build (nếu đang mở sẽ báo "already open").
Thẻ dùng trong content*.txt: H1/H2/H3 (tiêu đề), L (nhãn đậm), P (đoạn), B (gạch đầu dòng),
EQ (công thức), IMG|file|rộng_cm, IMG_PH (chỗ chèn hình), CAP/TCAP (chú thích), T|cột|cột (bảng),
CODE ... ENDCODE (khối mã, tự tô màu: từ khóa xanh dương, chú thích xanh lá).
Placeholder `[[...]]` sẽ thành `[...]` tô vàng — là chỗ cần điền.

> **QUAN TRỌNG trên Linux:** build.ps1 và figs.ps1 dùng Word COM + GDI+ nên KHÔNG chạy được.
> Trên Linux chỉ có thể sửa content*.txt và notebook. Muốn build .docx thì hoặc (a) quay lại
> Windows chạy build.ps1, hoặc (b) viết lại bộ build bằng python-docx / LibreOffice headless.

## Việc đang làm dở

1. **Notebook `Dự báo nhiệt độ/train_greenhouse_kaggle.ipynb` — ĐÃ CHỐT cách A, CHƯA sửa xong:**
   - Đổi `horizon_steps=6` → `horizon_steps=1` trong CONFIG (ô code 3). Lý do: firmware gọi mô hình
     theo bước 5 phút rồi lặp 6/12/18 lần (steps[] trong forecast.ino) để ra +30/+60/+90 phút.
     horizon=6 làm mô hình học mức thay đổi 30 phút → lệch. horizon=1 khớp với firmware.
   - Sửa ô vẽ `forecast_plot.png` (ô code 15): hiện dùng mô hình cuối (đã train trên chính dữ liệu
     vẽ → "tự chấm bài"). Cần vẽ dự báo ngoài mẫu từ bước kiểm định chéo (ô code 11) để khớp Bảng 2.13.
   - Sau khi sửa báo cáo: đổi "dự báo trước 30 phút" → "mỗi bước 5 phút, lặp cuốn chiếu tới +30/+60/+90".

2. **Chạy notebook trên Kaggle/Colab** (dữ liệu đã nén sẵn: `Dự báo nhiệt độ/Data_upload.zip`, 11 CSV).
   Cần lấy về để điền chỗ tô vàng trong 2.5_v2:
   - Bước 4: tổng số mẫu (cảm biến thật / Hà Nội)
   - Bước 5: bảng MAE tổng hợp → Bảng 2.13 + viết nhận xét
   - Bước 6: tổng số node 3 mô hình
   - Bước 7: ảnh forecast_plot.png → Hình 2.32
   - Chỗ còn lại: dung lượng Flash (phải biên dịch MasterNode trong Arduino IDE, có/không mô hình, lấy hiệu)
   - Số phụ lục chứa mã nguồn đầy đủ (đoạn mở đầu 2.5)

## Lỗi firmware/notebook còn tồn (user bảo "sửa sau", CHƯA sửa)

Ngoài lỗi horizon (đang sửa bằng cách A), còn các chỗ feature firmware tính khác lúc train:
- `ln(1+lux)` (np.log1p) khi train vs `log10f` trong firmware
- `light_sat6`: tỉ lệ mẫu bão hòa (train) vs tỉ lệ "có sáng" (firmware)
- `hour_sin/hour_cos`: firmware hardcode hourSin=0, hourCos=1 (không có RTC giờ thực?)
- `d12` dùng shift(12) vs bt[11]; `std` ddof=1 (pandas) vs chia /6 (firmware)
Nếu sửa triệt để thì phải train lại và chạy lại kịch bản 3.

## Tiếp tục trên Linux

- `git pull` để lấy thư mục này.
- Đọc HANDOFF.md này + giải nén transcript nếu cần xem lại chi tiết:
  `gunzip -k transcript_conversation.jsonl.gz`
- Claude Code không tự đồng bộ hội thoại giữa máy; bắt đầu phiên mới và đưa HANDOFF.md này cho nó
  là cách nối lại ngữ cảnh đáng tin cậy nhất. (File .jsonl resume được trên cùng máy, nhưng đường dẫn
  project hash khác giữa Windows/Linux nên `--resume` cross-OS không chắc chạy.)
