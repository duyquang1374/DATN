# PowerShell script to generate Concise & Academic Section 1.5 Word Document
# "1.5. CƠ SỞ LÝ THUYẾT VỀ TRÍ TUỆ NHÂN TẠO NHÚNG (TINYML / MACHINE LEARNING)"
# Phù hợp chuẩn Cơ sở lý thuyết: Tổng quan, gãy gọn, công thức cơ bản, có hình ảnh và nguồn rõ ràng.

$ErrorActionPreference = "Stop"

Get-Process winword -ErrorAction SilentlyContinue | Stop-Process -Force

Write-Host "Khoi tao Microsoft Word Application..."
$word = New-Object -ComObject Word.Application
$word.Visible = $false
$doc = $word.Documents.Add()
$sel = $word.Selection

# ─── 1. THIET LAP TRANG IN CHUAN DO AN TOT NGHIEP (A4) ───────────────────────
$cmToPt = 28.3465
$doc.PageSetup.PaperSize = 7 # wdPaperA4
$doc.PageSetup.TopMargin = 2.0 * $cmToPt
$doc.PageSetup.BottomMargin = 2.0 * $cmToPt
$doc.PageSetup.LeftMargin = 3.0 * $cmToPt
$doc.PageSetup.RightMargin = 2.0 * $cmToPt

# ─── CAC HAM TIEN ICH DINH DANG ──────────────────────────────────────────────
function Write-H1 {
    param([string]$text)
    $sel.Font.Name = "Times New Roman"
    $sel.Font.Size = 15
    $sel.Font.Bold = 1
    $sel.Font.Italic = 0
    $sel.Font.ColorIndex = 0
    $sel.ParagraphFormat.SpaceBefore = 14
    $sel.ParagraphFormat.SpaceAfter = 6
    $sel.ParagraphFormat.LineSpacingRule = 5
    $sel.ParagraphFormat.LineSpacing = 16
    $sel.ParagraphFormat.Alignment = 0 # Left
    $sel.ParagraphFormat.FirstLineIndent = 0
    $sel.ParagraphFormat.KeepWithNext = -1
    $sel.TypeText($text)
    $sel.TypeParagraph()
}

function Write-H2 {
    param([string]$text)
    $sel.Font.Name = "Times New Roman"
    $sel.Font.Size = 14
    $sel.Font.Bold = 1
    $sel.Font.Italic = 0
    $sel.Font.ColorIndex = 0
    $sel.ParagraphFormat.SpaceBefore = 10
    $sel.ParagraphFormat.SpaceAfter = 4
    $sel.ParagraphFormat.LineSpacingRule = 5
    $sel.ParagraphFormat.LineSpacing = 16
    $sel.ParagraphFormat.Alignment = 0
    $sel.ParagraphFormat.FirstLineIndent = 0
    $sel.ParagraphFormat.KeepWithNext = -1
    $sel.TypeText($text)
    $sel.TypeParagraph()
}

function Write-H3 {
    param([string]$text)
    $sel.Font.Name = "Times New Roman"
    $sel.Font.Size = 13
    $sel.Font.Bold = 1
    $sel.Font.Italic = 1
    $sel.Font.ColorIndex = 0
    $sel.ParagraphFormat.SpaceBefore = 8
    $sel.ParagraphFormat.SpaceAfter = 3
    $sel.ParagraphFormat.LineSpacingRule = 5
    $sel.ParagraphFormat.LineSpacing = 16
    $sel.ParagraphFormat.Alignment = 0
    $sel.ParagraphFormat.FirstLineIndent = 0
    $sel.ParagraphFormat.KeepWithNext = -1
    $sel.TypeText($text)
    $sel.TypeParagraph()
}

function Write-P {
    param([string]$text)
    $sel.Font.Name = "Times New Roman"
    $sel.Font.Size = 13
    $sel.Font.Bold = 0
    $sel.Font.Italic = 0
    $sel.Font.ColorIndex = 0
    $sel.ParagraphFormat.SpaceBefore = 0
    $sel.ParagraphFormat.SpaceAfter = 6
    $sel.ParagraphFormat.LineSpacingRule = 5
    $sel.ParagraphFormat.LineSpacing = 16
    $sel.ParagraphFormat.Alignment = 3 # Justify
    $sel.ParagraphFormat.FirstLineIndent = 28.35 # 1 cm indent
    $sel.ParagraphFormat.KeepWithNext = 0
    $sel.TypeText($text)
    $sel.TypeParagraph()
}

function Write-Bullet {
    param($boldPrefix, $text)
    if ($boldPrefix -is [System.Array]) {
        $text = [string]$boldPrefix[1]
        $boldPrefix = [string]$boldPrefix[0]
    } else {
        $boldPrefix = [string]$boldPrefix
        $text = [string]$text
    }
    $sel.Font.Name = "Times New Roman"
    $sel.Font.Size = 13
    $sel.Font.ColorIndex = 0
    $sel.ParagraphFormat.SpaceBefore = 0
    $sel.ParagraphFormat.SpaceAfter = 4
    $sel.ParagraphFormat.LineSpacingRule = 5
    $sel.ParagraphFormat.LineSpacing = 16
    $sel.ParagraphFormat.Alignment = 3
    $sel.ParagraphFormat.FirstLineIndent = 0
    $sel.ParagraphFormat.LeftIndent = 28.35
    $sel.ParagraphFormat.KeepWithNext = 0
    
    $sel.Font.Bold = 1
    $sel.TypeText("- " + $boldPrefix + ": ")
    $sel.Font.Bold = 0
    $sel.TypeText($text)
    $sel.TypeParagraph()
    $sel.ParagraphFormat.LeftIndent = 0
}

function Write-Eq {
    param($formula, $num)
    if ($formula -is [System.Array]) {
        $num = [string]$formula[1]
        $formula = [string]$formula[0]
    } else {
        $formula = [string]$formula
        $num = [string]$num
    }
    $tbl = $doc.Tables.Add($sel.Range, 1, 2)
    $tbl.Borders.Enable = $false
    $tbl.Columns.Item(1).Width = 380
    $tbl.Columns.Item(2).Width = 70
    
    $c1 = $tbl.Cell(1, 1).Range
    $c1.Font.Name = "Cambria Math"
    $c1.Font.Size = 12
    $c1.Font.Italic = 1
    $c1.ParagraphFormat.Alignment = 1 # Center
    $c1.ParagraphFormat.SpaceBefore = 4
    $c1.ParagraphFormat.SpaceAfter = 4
    $c1.ParagraphFormat.FirstLineIndent = 0
    $c1.Text = $formula
    
    $c2 = $tbl.Cell(1, 2).Range
    $c2.Font.Name = "Times New Roman"
    $c2.Font.Size = 12
    $c2.Font.Italic = 0
    $c2.ParagraphFormat.Alignment = 2 # Right
    $c2.ParagraphFormat.SpaceBefore = 4
    $c2.ParagraphFormat.SpaceAfter = 4
    $c2.ParagraphFormat.FirstLineIndent = 0
    $c2.Text = "(" + $num + ")"
    
    $sel.SetRange($tbl.Range.End, $tbl.Range.End)
    $sel.TypeParagraph()
}

function Write-TableCaption {
    param([string]$caption)
    $sel.Font.Name = "Times New Roman"
    $sel.Font.Size = 12
    $sel.Font.Bold = 1
    $sel.Font.Italic = 0
    $sel.ParagraphFormat.SpaceBefore = 8
    $sel.ParagraphFormat.SpaceAfter = 3
    $sel.ParagraphFormat.Alignment = 1 # Center
    $sel.ParagraphFormat.FirstLineIndent = 0
    $sel.ParagraphFormat.KeepWithNext = -1
    $sel.TypeText($caption)
    $sel.TypeParagraph()
}

function Write-ImageWithSource {
    param([string]$imagePath, [string]$caption, [string]$source, [float]$widthPt, [float]$heightPt)
    if (Test-Path $imagePath) {
        $sel.ParagraphFormat.Alignment = 1 # Center
        $sel.ParagraphFormat.SpaceBefore = 8
        $sel.ParagraphFormat.SpaceAfter = 2
        $sel.ParagraphFormat.FirstLineIndent = 0
        $sel.ParagraphFormat.KeepWithNext = -1
        
        $shape = $sel.InlineShapes.AddPicture($imagePath)
        $shape.Width = $widthPt
        $shape.Height = $heightPt
        $sel.TypeParagraph()
        
        # Caption
        $sel.Font.Name = "Times New Roman"
        $sel.Font.Size = 11.5
        $sel.Font.Bold = 1
        $sel.Font.Italic = 0
        $sel.ParagraphFormat.SpaceBefore = 3
        $sel.ParagraphFormat.SpaceAfter = 1
        $sel.ParagraphFormat.Alignment = 1
        $sel.ParagraphFormat.KeepWithNext = -1
        $sel.TypeText($caption)
        $sel.TypeParagraph()
        
        # Source citation
        $sel.Font.Name = "Times New Roman"
        $sel.Font.Size = 10.5
        $sel.Font.Bold = 0
        $sel.Font.Italic = 1
        $sel.ParagraphFormat.SpaceBefore = 0
        $sel.ParagraphFormat.SpaceAfter = 8
        $sel.ParagraphFormat.Alignment = 1
        $sel.ParagraphFormat.KeepWithNext = 0
        $sel.TypeText($source)
        $sel.TypeParagraph()
    }
}

# ─────────────────────────────────────────────────────────────────────────────
# BẮT ĐẦU NỘI DUNG MỤC 1.5 (TỔNG QUAN CƠ SỞ LÝ THUYẾT)
# ─────────────────────────────────────────────────────────────────────────────

Write-H1("1.5. CƠ SỞ LÝ THUYẾT VỀ TRÍ TUỆ NHÂN TẠO NHÚNG (TINYML / MACHINE LEARNING)")

Write-P("Trong các hệ thống nông nghiệp thông minh thế hệ mới, việc kiểm soát vi khí hậu không chỉ dừng lại ở các phản ứng đóng/ngắt cơ bản khi môi trường vượt ngưỡng. Sự kết hợp giữa Trí tuệ nhân tạo (AI) và Hệ thống nhúng (Embedded Systems) - thường được gọi là TinyML (Tiny Machine Learning) - mang đến khả năng dự báo trước sự thay đổi của môi trường để thực hiện điều khiển đón đầu (Predictive Control). Mục này trình bày tổng quan các khái niệm nền tảng, bài toán dự báo chuỗi thời gian, nguyên lý các giải thuật học máy dạng cây và quy trình triển khai mô hình học máy trên vi điều khiển. Các thuật toán chi tiết và mã nguồn thực thi sẽ được đề cập chuyên sâu tại Chương Thiết kế phần mềm.")

# ── 1.5.1
Write-H2("1.5.1. Khái niệm và vị trí của Trí tuệ nhân tạo nhúng (TinyML)")
Write-P("TinyML là lĩnh vực nghiên cứu giao thoa giữa Trí tuệ nhân tạo (Học máy) và Hệ thống phần cứng nhúng. Mục tiêu của TinyML là tối ưu hóa và đưa các mô hình học máy hoạt động trực tiếp trên các bộ vi điều khiển (MCU) công suất thấp (vài miliwatt đến dưới 1 watt) với dung lượng bộ nhớ giới hạn tính bằng kilobyte (KB).")

Write-P("Trong các ứng dụng nông nghiệp công nghệ cao, nhà màng thường được lắp đặt tại các vùng ngoại ô hoặc nông thôn, nơi hạ tầng viễn thông thường xuyên chập chờn. Việc triển khai mô hình học máy tại biên (On-device TinyML) mang lại những ưu thế vượt bậc so với mô hình AI trên đám mây (Cloud-based AI) truyền thống:")

Write-Bullet("Hoạt động hoàn toàn tự chủ (Zero-connectivity Reliability)", "Hệ thống tự thực hiện suy luận cục bộ trên vi điều khiển mà không cần kết nối Internet, đảm bảo chu trình giám sát và điều khiển không bị gián đoạn khi xảy ra sự cố mạng.")
Write-Bullet("Độ trễ phản hồi tức thời (Real-time Latency)", "Thời gian suy luận trực tiếp trên vi điều khiển diễn ra trong vòng vài mili-giây, phản ứng nhanh hơn rất nhiều so với việc gửi gói tin lên Cloud rồi đợi phản hồi.")
Write-Bullet("Tiết kiệm chi phí và năng lượng", "Loại bỏ hoàn toàn chi phí thuê máy chủ đám mây định kỳ, giảm thiểu năng lượng tiêu hao cho các module vô tuyến công suất cao.")

$img1 = "C:\Users\Quang\Documents\CODE\DATN\figures\hinh_1_5_1_tinyml_concept.png"
Write-ImageWithSource $img1 "Hình 1.5: Sơ đồ so sánh luồng xử lý giữa Điện toán đám mây (Cloud AI) và Trí tuệ nhân tạo nhúng (TinyML) trong nông nghiệp" "(Nguồn: Tổng hợp từ IEEE Internet of Things Journal và Pete Warden, O'Reilly Media)" 440 220

Write-TableCaption("Bảng 1.1: So sánh tổng quan giữa kiến trúc Cloud-based AI và TinyML trong bài toán Nhà kính")

$t1Data = @(
    @("Tiêu chí đánh giá", "Mô hình Điện toán Đám mây (Cloud AI)", "Mô hình Nhúng tại biên (TinyML)"),
    @("Độ phụ thuộc mạng Internet", "Bắt buộc 100%, ngừng trệ khi mất kết nối", "Hoàn toàn độc lập, hoạt động ngoại tuyến (Offline)"),
    @("Độ trễ xử lý (Latency)", "Cao và biến động (500 ms – vài giây)", "Rất thấp, có tính tất định (< 5 ms trên ESP32)"),
    @("Chi phí vận hành", "Phát sinh phí thuê máy chủ và lưu lượng 4G", "Bằng 0 VNĐ sau khi nạp firmware vào vi điều khiển"),
    @("Tính an toàn dữ liệu", "Dữ liệu truyền lên máy chủ bên thứ ba", "Dữ liệu được lưu trữ và tính toán tại chỗ")
)

$t1 = $doc.Tables.Add($sel.Range, $t1Data.Count, 3)
$t1.Borders.Enable = $true
$t1.Borders.OutsideColor = 0x666666
$t1.Borders.InsideColor = 0xCCCCCC
$t1.Rows.Item(1).Range.Font.Bold = 1
$t1.Rows.Item(1).Range.ParagraphFormat.Alignment = 1
$t1.Rows.Item(1).Shading.BackgroundPatternColor = 0xE0E0E0

for ($r = 1; $r -le $t1Data.Count; $r++) {
    for ($c = 1; $c -le 3; $c++) {
        $cell = $t1.Cell($r, $c).Range
        $cell.Text = $t1Data[$r-1][$c-1]
        $cell.Font.Name = "Times New Roman"
        $cell.Font.Size = 11
        $cell.ParagraphFormat.SpaceBefore = 2
        $cell.ParagraphFormat.SpaceAfter = 2
        $cell.ParagraphFormat.LineSpacingRule = 0
        if ($r -gt 1) {
            if ($c -eq 1) { $cell.Font.Bold = 1; $cell.ParagraphFormat.Alignment = 0 }
            else { $cell.ParagraphFormat.Alignment = 3 }
        }
    }
}
$sel.SetRange($t1.Range.End, $t1.Range.End)
$sel.TypeParagraph()

# ── 1.5.2
Write-H2("1.5.2. Khái niệm bài toán dự báo chuỗi thời gian vi khí hậu (Time-Series Forecasting)")
Write-P("Vi khí hậu nhà kính có đặc tính quán tính nhiệt (Thermal Inertia) lớn: khi bức xạ mặt trời chiếu mạnh vào buổi trưa, không khí bên trong không nóng lên ngay lập tức mà tích tụ dần qua hiệu ứng nhà kính và sẽ tăng vọt sau một khoảng thời gian trễ. Nếu chỉ sử dụng các thuật toán điều khiển phản hồi cổ điển (chỉ kích hoạt quạt khi nhiệt độ đã vượt ngưỡng), nhiệt độ thực tế vẫn sẽ tiếp tục tăng do quán tính nhiệt, gây sốc nhiệt cho cây trồng.")

Write-P("Bài toán dự báo chuỗi thời gian (Time-Series Forecasting) được định nghĩa là việc ước lượng trạng thái tương lai y_hat tại thời điểm (t + h) dựa trên chuỗi các giá trị quan sát lịch sử từ quá khứ đến thời điểm hiện tại:")

Write-Eq("y_hat(t+h) = f( x(t), x(t-1), x(t-2), ..., x(t-k) )", "1.1")

Write-P("Trong đó: x(t) là các thông số vi khí hậu quan trắc được tại thời điểm t (Nhiệt độ, Độ ẩm không khí, Cường độ ánh sáng); k là độ dài cửa sổ lịch sử quan sát; và h là chân trời dự báo trong tương lai (ví dụ: trước 30 phút hoặc 60 phút). Thông qua giá trị dự báo này, hệ thống có thể kích hoạt quạt hút hoặc mở rèm che nắng sớm từ trước, giúp dập tắt đỉnh quá nhiệt và duy trì môi trường ổn định cho cây trồng.")

# ── 1.5.3
Write-H2("1.5.3. Kỹ thuật mã hóa và trích xuất đặc trưng cơ bản (Feature Engineering)")
Write-P("Dữ liệu cảm biến thô nếu đưa trực tiếp vào mô hình thường khó đạt độ chính xác cao. Kỹ thuật trích xuất đặc trưng giúp chuyển đổi dữ liệu chuỗi thời gian thành các thông tin có ý nghĩa vật lý rõ ràng:")

Write-Bullet("Đặc trưng trễ (Lag Features)", "Lưu trữ giá trị của các bước thời gian trước đó nhằm phản ánh vận tốc và gia tốc biến thiên của nhiệt ẩm:")
Write-Eq("x_lag(t) = x(t - k)", "1.2")

Write-Bullet("Đặc trưng thống kê trượt (Rolling Statistics)", "Tính toán giá trị trung bình trượt để lọc nhiễu tần số cao của cảm biến và độ lệch chuẩn trượt để đánh giá mức độ bất ổn định của thời tiết trong một khoảng thời gian gần nhất.")

Write-Bullet("Mã hóa chu kỳ thời gian lượng giác (Cyclical Time Encoding)", "Do chu kỳ ngày - đêm có tính tuần hoàn 24 giờ, việc dùng biến thời gian tuyến tính từ 0 đến 23 sẽ tạo ra bước nhảy gián đoạn phi lý giữa 23h đêm và 0h sáng. Để khắc phục, thời gian được ánh xạ lên vòng tròn lượng giác hai chiều thông qua hàm sin và cos:")

Write-Eq("hour_sin = sin( 2π * t_hour / 24 ),    hour_cos = cos( 2π * t_hour / 24 )", "1.3")

Write-P("Phép biến đổi này giúp khoảng cách giữa 23h59 và 00h00 luôn liên tục và trơn tru, giúp mô hình học máy nắm bắt chính xác quy luật sinh học ngày - đêm.")

$img3 = "C:\Users\Quang\Documents\CODE\DATN\figures\hinh_1_5_3_cyclical_encoding.png"
Write-ImageWithSource $img3 "Hình 1.7: Biểu diễn chu kỳ thời gian 24 giờ trên vòng tròn lượng giác hai chiều (Cyclical Encoding)" "(Nguồn: Scikit-learn Documentation & Ian Goodfellow et al., Deep Learning, MIT Press)" 440 207

# ── 1.5.4
Write-H2("1.5.4. Nguyên lý các mô hình học máy dạng cây (Tree-based Models)")
Write-P("Trong các bài toán học máy nhúng trên vi điều khiển, các mô hình học máy dạng cây (Tree-based Models) được ưu tiên sử dụng hàng đầu nhờ cấu trúc tính toán đơn giản, không đòi hỏi các phép nhân ma trận nặng nề như mạng nơ-ron sâu.")

Write-H3("1.5.4.1. Cây quyết định hồi quy (Decision Tree Regressor)")
Write-P("Cây quyết định hoạt động bằng cách phân chia không gian dữ liệu thành các tập con thông qua các câu hỏi điều kiện dạng nhị phân tại mỗi nút (ví dụ: 'Nhiệt độ hiện tại có nhỏ hơn 32°C hay không?'). Quá trình đi từ nút gốc (Root Node) qua các nút điều kiện rẽ nhánh đến nút lá (Leaf Node) sẽ cho ra giá trị dự báo cuối cùng. Tuy nhiên, một cây quyết định đơn lẻ thường rất dễ bị hiện tượng quá khớp (Overfitting) và nhạy cảm với nhiễu đo lường.")

Write-H3("1.5.4.2. Rừng cây ngẫu nhiên (Random Forest Regressor)")
Write-P("Để khắc phục nhược điểm của cây đơn lẻ, giải thuật Rừng ngẫu nhiên (Random Forest) kết hợp nhiều cây quyết định độc lập lại với nhau thông qua kỹ thuật học tập hợp (Ensemble Learning). Mỗi cây được huấn luyện trên một tập dữ liệu ngẫu nhiên có hoàn lại (Bagging) và chỉ xét một tập con ngẫu nhiên các đặc trưng tại mỗi lần phân nhánh.")

Write-P("Kết quả dự báo cuối cùng của toàn bộ mô hình là giá trị trung bình cộng từ kết quả dự báo của B cây quyết định thành phần:")

Write-Eq("y_hat_RF = (1 / B) * ∑_{b=1}^{B} T_b(x)", "1.4")

Write-P("Nhờ cơ chế trung bình hóa này, các sai số ngẫu nhiên của từng cây sẽ triệt tiêu lẫn nhau, giúp giảm thiểu phương sai (Variance Reduction) và mang lại khả năng tổng quát hóa xuất sắc trên dữ liệu thực tế.")

$img2 = "C:\Users\Quang\Documents\CODE\DATN\figures\hinh_1_5_2_decision_tree_rf.png"
Write-ImageWithSource $img2 "Hình 1.6: Nguyên lý cấu tạo của Cây quyết định và cơ chế tổng hợp kết quả của Rừng ngẫu nhiên (Random Forest)" "(Nguồn: Leo Breiman, Random Forests, Machine Learning Journal, 2001)" 440 210

Write-TableCaption("Bảng 1.2: So sánh đặc tính giữa Cây quyết định đơn lẻ và Rừng ngẫu nhiên")

$t2Data = @(
    @("Đặc tính so sánh", "Cây quyết định đơn lẻ (Decision Tree)", "Rừng ngẫu nhiên (Random Forest)"),
    @("Cấu trúc mô hình", "Chỉ gồm 1 cây phân nhánh nhị phân", "Tập hợp gồm nhiều cây quyết định độc lập"),
    @("Nguy cơ quá khớp (Overfitting)", "Cao (dễ bị học vẹt theo nhiễu dữ liệu)", "Rất thấp (nhờ kỹ thuật lấy mẫu ngẫu nhiên và trung bình hóa)"),
    @("Độ ổn định và chính xác", "Kém ổn định khi dữ liệu đầu vào biến động", "Độ chính xác và độ ổn định vượt trội"),
    @("Khả năng thực thi trên MCU", "Cực nhẹ, tốc độ tức thì", "Rất phù hợp, thực thi bằng các lệnh so sánh rẽ nhánh C99")
)

$t2 = $doc.Tables.Add($sel.Range, $t2Data.Count, 3)
$t2.Borders.Enable = $true
$t2.Borders.OutsideColor = 0x666666
$t2.Borders.InsideColor = 0xCCCCCC
$t2.Rows.Item(1).Range.Font.Bold = 1
$t2.Rows.Item(1).Range.ParagraphFormat.Alignment = 1
$t2.Rows.Item(1).Shading.BackgroundPatternColor = 0xE0E0E0

for ($r = 1; $r -le $t2Data.Count; $r++) {
    for ($c = 1; $c -le 3; $c++) {
        $cell = $t2.Cell($r, $c).Range
        $cell.Text = $t2Data[$r-1][$c-1]
        $cell.Font.Name = "Times New Roman"
        $cell.Font.Size = 11
        $cell.ParagraphFormat.SpaceBefore = 2
        $cell.ParagraphFormat.SpaceAfter = 2
        $cell.ParagraphFormat.LineSpacingRule = 0
        if ($r -gt 1) {
            if ($c -eq 1) { $cell.Font.Bold = 1; $cell.ParagraphFormat.Alignment = 0 }
            else { $cell.ParagraphFormat.Alignment = 3 }
        }
    }
}
$sel.SetRange($t2.Range.End, $t2.Range.End)
$sel.TypeParagraph()

# ── 1.5.5
Write-H2("1.5.5. Quy trình tổng quan triển khai mô hình học máy nhúng (TinyML Pipeline)")
Write-P("Để đưa một mô hình học máy từ môi trường nghiên cứu trên máy tính xuống hoạt động ổn định trên vi điều khiển ESP32, quy trình triển khai được tiến hành theo 4 giai đoạn nối tiếp:")

Write-Bullet("1. Thu thập và tiền xử lý dữ liệu", "Ghi nhận dữ liệu nhiệt độ, độ ẩm và ánh sáng từ trạm cảm biến thực địa, loại bỏ ngoại lai và trích xuất các đặc trưng chuỗi thời gian.")
Write-Bullet("2. Huấn luyện mô hình trên Python", "Sử dụng thư viện Scikit-learn để huấn luyện mô hình Random Forest Regressor và kiểm chứng độ chính xác trên tập kiểm thử độc lập.")
Write-Bullet("3. Chuyển đổi mô hình sang C99 (emlearn)", "Sử dụng framework emlearn để biên dịch toàn bộ cấu trúc cây thành các câu lệnh rẽ nhánh tĩnh if-else thuần túy trong ngôn ngữ C. Cấu trúc này được lưu trữ trong bộ nhớ Flash của vi điều khiển, giúp tiêu hao RAM thực tế gần như bằng 0.")
Write-Bullet("4. Nạp firmware và thực thi trên MCU", "Tích hợp mã nguồn C đã sinh vào chương trình điều khiển của ESP32 để định kỳ thực hiện suy luận dự báo vi khí hậu phục vụ điều khiển quạt, sưởi và mái che.")

$img4 = "C:\Users\Quang\Documents\CODE\DATN\figures\hinh_1_5_4_tinyml_pipeline.png"
Write-ImageWithSource $img4 "Hình 1.8: Quy trình tổng quan các bước phát triển và triển khai mô hình học máy nhúng (TinyML Pipeline)" "(Nguồn: Tác giả tổng hợp theo kiến trúc triển khai hệ thống)" 440 160

# ── 1.5.6
Write-H2("1.5.6. Các chỉ số cơ bản đánh giá mô hình hồi quy vi khí hậu")
Write-P("Để đánh giá độ chính xác của mô hình dự báo hồi quy, ba chỉ số toán học thống kê cơ bản được sử dụng phổ biến:")

Write-H3("1.5.6.1. Sai số tuyệt đối trung bình (MAE - Mean Absolute Error)")
Write-P("MAE phản ánh độ lệch trung bình giữa giá trị dự báo và giá trị đo thực tế, có cùng đơn vị đo với đại lượng khảo sát (°C hoặc %):")

Write-Eq("MAE = (1 / N) * ∑_{i=1}^{N} | y_i - y_hat_i |", "1.5")

Write-H3("1.5.6.2. Căn bậc hai sai số bình phương trung bình (RMSE - Root Mean Squared Error)")
Write-P("RMSE có xu hướng trừng phạt nặng hơn các điểm sai lệch lớn nhờ phép bình phương trước khi lấy căn, giúp phát hiện các tình huống dự báo sai lệch bất thường:")

Write-Eq("RMSE = sqrt( (1 / N) * ∑_{i=1}^{N} ( y_i - y_hat_i )^2 )", "1.6")

Write-H3("1.5.6.3. Hệ số xác định (R² - Coefficient of Determination)")
Write-P("Chỉ số R² thể hiện mức độ phù hợp của mô hình với dữ liệu thực tế (với R² = 1 là dự báo hoàn hảo, R² > 0.8 là mô hình đạt chất lượng tốt đối với chuỗi thời gian khí tượng vi mô).")

Write-H3("1.5.6.4. Tóm tắt kết luận mục 1.5")
Write-P("Tóm lại, việc ứng dụng mô hình Trí tuệ nhân tạo nhúng (TinyML) dựa trên thuật toán Rừng ngẫu nhiên (Random Forest) và công nghệ biên dịch mã C99 của emlearn tạo nền tảng lý thuyết vững chắc cho bài toán điều khiển đón đầu vi khí hậu nhà màng. Cấu trúc mô hình gọn nhẹ, tính toán nhanh và không phụ thuộc vào đường truyền mạng Internet là giải pháp tối ưu cho nông nghiệp công nghệ cao. Các bước thiết kế tập đặc trưng cụ thể, thông số huấn luyện chi tiết và mã nguồn C nhúng vào vi điều khiển sẽ được trình bày chuyên sâu tại các chương sau.")

# ─── LƯU TỆP TIN VÀ DỌN DẸP TIẾN TRÌNH ─────────────────────────────────────────
$outPath = "C:\Users\Quang\Documents\CODE\DATN\BaoCao_Phan_1.5_TinyML.docx"
Write-Host "Luu tep tin vao: $outPath..."

$doc.GetType().InvokeMember("SaveAs2", [System.Reflection.BindingFlags]::InvokeMethod, $null, $doc, [object[]]@($outPath))
$doc.Close()
$word.Quit()
[System.Runtime.Interopservices.Marshal]::ReleaseComObject($word) | Out-Null
[System.GC]::Collect()
[System.GC]::WaitForPendingFinalizers()

if (Test-Path $outPath) {
    $fileInfo = Get-Item $outPath
    Write-Host "HOAN THANH! Dung luong tep: $($fileInfo.Length) bytes."
} else {
    Write-Error "Khong tim thay tep sau khi luu!"
}

