# VolumeMath — 3D Slicer Extension for Medical Image Arithmetic & Evaluation

![Platform](https://img.shields.io/badge/Platform-3D%20Slicer%205.8-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C?logo=cplusplus)
![UI](https://img.shields.io/badge/UI-Qt-41CD52?logo=qt)
![Lib](https://img.shields.io/badge/Lib-VTK%20%2F%20ITK-orange)
![OS](https://img.shields.io/badge/OS-Windows-blue?logo=windows)

---

## Overview

3D Slicer 上で医用画像の演算処理・定量評価を行うカスタム拡張モジュール。

従来 ImageJ で個別に実施していた画像演算と評価指標計算（MSE・NCC）を 3D Slicer 環境に統合し、放射線治療・画像比較ワークフローを一元化。VTK パイプラインベースの実装により、spacing / origin などのメタデータを維持したまま処理を行う。

---

## Tech Stack

| 項目 | 技術 |
|---|---|
| 言語 | C++ |
| UI | Qt |
| 画像処理 | VTK / ITK |
| プラットフォーム | 3D Slicer 5.8 API |
| 対応 OS | Windows |

---

## Features

### 画像演算

| 演算 | 内容 |
|---|---|
| Add / Subtract | 加算・差分 |
| Multiply / Divide | 乗算・除算 |
| Min / Max | 最小・最大値演算 |
| Square / Square Root | 2乗・平方根 |
| Absolute | 絶対値 |

### 論理演算

`AND` / `OR` / `NOT` / `XOR`

---

### 評価指標

#### MSE（Mean Squared Error）

$$
MSE = \frac{1}{N} \sum_{i=1}^{N} \left( I_1(i) - I_2(i) \right)^2
$$

2画像間の画素値差の二乗平均。線量差・画像レジストレーション精度の定量評価に使用。

#### NCC（Normalized Cross-Correlation）

$$
NCC = \frac{\sum_{i=1}^{N} I_1(i) \cdot I_2(i)}
{\sqrt{\sum_{i=1}^{N} I_1(i)^2} \cdot \sqrt{\sum_{i=1}^{N} I_2(i)^2}}
$$

値域：$[-1, 1]$　— 1 に近いほど類似度が高い。輝度変化に対してロバストな類似度評価に有効。

---

## Implementation Notes

- **VTK パイプライン**による処理で spacing / origin などのメタデータを維持
- 単一画像・2画像入力の両方に対応
- 出力先：既存ボリュームへの上書き または 新規ボリューム生成

> **Note:** DICOM 出力時は int16 変換が発生するため、float 精度を保持する場合は **NRRD 形式**を推奨。

---

## Usage

```
1. zip から Extension をインストール
2. DICOM Import から CT ボリュームを読み込み
3. 入力ボリュームを指定
4. 演算 / 評価指標を選択して実行
```

---

## Screenshot

![画面](use_imgs.png)

---

## Use Cases

- DIR（変形画像レジストレーション）結果の定量評価
- 放射線治療における計画 CT と CBCT の画像比較
- ガンマ解析の前処理（差分画像生成）

---