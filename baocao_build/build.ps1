param([string]$ContentFile, [string]$MediaDir, [string]$OutFile)
$ErrorActionPreference = "Stop"
$lines = Get-Content -LiteralPath $ContentFile -Encoding UTF8

$word = New-Object -ComObject Word.Application
$word.Visible = $false
$doc = $word.Documents.Add()
$sel = $word.Selection
$cm = 28.3465
$doc.PageSetup.PaperSize = 7
$doc.PageSetup.TopMargin = 2.0 * $cm
$doc.PageSetup.BottomMargin = 2.0 * $cm
$doc.PageSetup.LeftMargin = 3.0 * $cm
$doc.PageSetup.RightMargin = 2.0 * $cm

function Set-Fmt($size, $bold, $italic, $align, $indent, $before, $after, $keep) {
    $sel.Font.Name = "Times New Roman"
    $sel.Font.Size = $size
    $sel.Font.Bold = $bold
    $sel.Font.Italic = $italic
    $sel.Font.ColorIndex = 0
    $pf = $sel.ParagraphFormat
    $pf.Alignment = [int]$align
    $pf.LeftIndent = [single]0
    $pf.FirstLineIndent = [single]$indent
    $pf.SpaceBefore = [single]$before
    $pf.SpaceAfter = [single]$after
    $pf.LineSpacingRule = 1   # 1.5 lines
    $pf.KeepWithNext = [int]$keep
    $pf.Shading.BackgroundPatternColor = -16777216
    $pf.Borders.Enable = 0
}

try {
$i = 0
while ($i -lt $lines.Count) {
    $line = $lines[$i]
    if ([string]::IsNullOrWhiteSpace($line)) { $i++; continue }
    $tag, $rest = $line.Split('|', 2)
    switch ($tag) {
        'H1' { Set-Fmt 14 1 0 1 0 12 12 -1; $sel.TypeText($rest.ToUpper()); $sel.TypeParagraph() }
        'H2' { Set-Fmt 13 1 0 3 0 12 6 -1; $sel.TypeText($rest); $sel.TypeParagraph() }
        'H3' { Set-Fmt 13 1 1 3 0 6 3 -1; $sel.TypeText($rest); $sel.TypeParagraph() }
        'L'  { Set-Fmt 13 1 0 3 0 3 3 -1; $sel.TypeText($rest); $sel.TypeParagraph() }
        'P'  { Set-Fmt 13 0 0 3 (1.0 * $cm) 0 6 0; $sel.TypeText($rest); $sel.TypeParagraph() }
        'EQ' { Set-Fmt 13 0 1 1 0 3 6 0; $sel.TypeText($rest); $sel.TypeParagraph() }
        'B'  {
            Set-Fmt 13 0 0 3 0 0 3 0
            $sel.ParagraphFormat.LeftIndent = 1.0 * $cm
            $sel.ParagraphFormat.FirstLineIndent = -0.5 * $cm
            $sel.TypeText([string][char]0x2013 + "`t" + $rest)
            $sel.TypeParagraph()
        }
        'IMG' {
            $parts = $rest.Split('|')
            $files = $parts[0].Split(';')
            $w = [double]$parts[1] * $cm
            Set-Fmt 13 0 0 1 0 6 0 -1
            $k = 0
            foreach ($f in $files) {
                $shp = $sel.InlineShapes.AddPicture((Join-Path $MediaDir $f), $false, $true)
                $shp.LockAspectRatio = -1
                $shp.Width = $w
                $k++
                if ($k -lt $files.Count) { $sel.TypeText("  ") }
            }
            $sel.TypeParagraph()
        }
        'CODE' {
            $buf = New-Object System.Collections.Generic.List[string]
            $i++
            while ($i -lt $lines.Count -and $lines[$i] -ne 'ENDCODE') { $buf.Add($lines[$i]); $i++ }
            Set-Fmt 9 0 0 0 0 0 0 0
            $r = $sel.Range
            $r.Text = ($buf -join "`r")
            $r.Font.Name = "Consolas"
            $r.Font.Size = 9
            $r.Font.Bold = 0
            $r.Font.Italic = 0
            $r.Font.Color = 0
            $r.ParagraphFormat.Alignment = 0
            $r.ParagraphFormat.FirstLineIndent = 0
            $r.ParagraphFormat.LeftIndent = 0.2 * $cm
            $r.ParagraphFormat.SpaceBefore = 0
            $r.ParagraphFormat.SpaceAfter = 0
            $r.ParagraphFormat.LineSpacingRule = 0
            $r.ParagraphFormat.Shading.BackgroundPatternColor = 15921906
            $r.ParagraphFormat.Borders.Enable = 1
            $r.ParagraphFormat.Borders.OutsideColor = 12632256
            $r.NoProofing = 1
            $cs = $r.Start; $ce = $r.End

            # --- Syntax highlighting ---
            $ct = ($buf -join "`n")
            $isC = $ct -match '(void |float |int32_t|static |#define |#include)'
            $isPy = $ct -match '(?m)(^def |^import |^from |pd\.|np\.|joblib\.|emlearn\.)'
            $blue = 16711680; $green = 32768

            if ($isC) {
                $kws = @('static','const','int','float','double','void','char',
                    'return','if','else','for','while','break','continue',
                    'sizeof','inline','int32_t','struct','File','true','false','NULL')
            } elseif ($isPy) {
                $kws = @('def','class','import','from','return','if','elif','else',
                    'for','while','in','not','and','or','is','None','True','False',
                    'with','as','dict','float','int','bool')
            } else {
                $kws = @('function','foreach','for','if','else','return')
            }
            foreach ($kw in $kws) {
                $sr = $doc.Range($cs, $ce)
                $f = $sr.Find
                $f.ClearFormatting(); $f.Replacement.ClearFormatting()
                $f.Replacement.Font.Color = $blue
                $f.Execute($kw, $true, $true, $false, $false, $false, $true, 0, $true, $kw, 2) | Out-Null
            }

            # Comments green (after keywords so green overrides blue inside comments)
            $marker = if ($isC) { '//' } else { '#' }
            $pos = $cs
            foreach ($ln in $buf) {
                $k = $ln.IndexOf($marker)
                if ($k -ge 0) { $doc.Range($pos + $k, $pos + $ln.Length).Font.Color = $green }
                $pos += $ln.Length + 1
            }

            $sel.SetRange($ce, $ce)
            $sel.TypeParagraph()
            Set-Fmt 12 0 1 1 0 3 9 0
        }
        'IMG_PH' { Set-Fmt 13 0 1 1 0 18 18 -1; $sel.TypeText($rest); $sel.TypeParagraph() }
        'CAP'  { Set-Fmt 12 0 1 1 0 3 9 0; $sel.TypeText($rest); $sel.TypeParagraph() }
        'TCAP' { Set-Fmt 12 0 1 1 0 6 3 -1; $sel.TypeText($rest); $sel.TypeParagraph() }
        'T' {
            $rows = @()
            while ($i -lt $lines.Count -and $lines[$i].StartsWith('T|')) {
                $rows += ,($lines[$i].Substring(2).Split('|')); $i++
            }
            $nc = $rows[0].Count
            Set-Fmt 12 0 0 0 0 0 0 0
            $sel.ParagraphFormat.LineSpacingRule = 0
            $tbl = $doc.Tables.Add($sel.Range, $rows.Count, $nc)
            $tbl.Borders.Enable = 1
            $tbl.Range.Font.Name = "Times New Roman"
            $tbl.Range.Font.Size = 12
            $tbl.Range.ParagraphFormat.FirstLineIndent = 0
            $tbl.Range.ParagraphFormat.SpaceAfter = 0
            $tbl.Range.ParagraphFormat.LineSpacingRule = 0
            for ($r = 0; $r -lt $rows.Count; $r++) {
                for ($c = 0; $c -lt $nc; $c++) {
                    $tbl.Cell($r + 1, $c + 1).Range.Text = $rows[$r][$c]
                }
            }
            $tbl.Rows.Item(1).Range.Font.Bold = 1
            $tbl.Rows.Item(1).Range.ParagraphFormat.Alignment = 1
            $tbl.Rows.Item(1).Shading.BackgroundPatternColor = 14277081
            $tbl.Rows.Item(1).HeadingFormat = -1
            $tbl.PreferredWidthType = 2
            $tbl.PreferredWidth = 100
            $tbl.AutoFitBehavior(1)
            $tbl.AutoFitBehavior(2)
            $sel.EndKey(6) | Out-Null
            Set-Fmt 13 0 0 3 0 0 6 0
            $sel.TypeParagraph()
            continue
        }
    }
    $i++
}

# Highlight placeholders [[...]] -> [...] in yellow
$word.Options.DefaultHighlightColorIndex = 7
$rng = $doc.Content
$f = $rng.Find
$f.ClearFormatting()
$f.Replacement.ClearFormatting()
$f.Replacement.Highlight = 1
$f.Execute('\[\[(*)\]\]', $false, $false, $true, $false, $false, $true, 1, $true, '[\1]', 2) | Out-Null

$doc.SaveAs2($OutFile, 16)
Write-Host "Saved: $OutFile"
} finally {
    try { $doc.Close(0) } catch {}
    $word.Quit()
}
