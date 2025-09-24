import pandas as pd
import matplotlib.pyplot as plt
import os
import glob
from datetime import datetime

import platform

# 한글 폰트 설정
if platform.system() == 'Windows':
    plt.rc('font', family='Malgun Gothic')
elif platform.system() == 'Darwin': # Mac
    plt.rc('font', family='AppleGothic')
else: # Linux
    # For Linux, you may need to install a Korean font like NanumGothic
    # sudo apt-get install fonts-nanum*
    plt.rc('font', family='NanumGothic')

# 마이너스 기호 깨짐 방지
plt.rcParams['axes.unicode_minus'] = False

def analyze_sensor_data(filename=None):
    """
    수집된 센서 데이터를 분석하고 시각화
    
    Args:
        filename (str): 분석할 CSV 파일명 (기본값: 가장 최신 파일)
    """
    
    data_dir = "data"
    csv_dir = os.path.join(data_dir, "csv_files")
    
    # 파일 선택
    if filename is None:
        # 가장 최신 파일 찾기
        csv_files = glob.glob(os.path.join(csv_dir, "sensor_data_*.csv"))
        if not csv_files:
            print("분석할 데이터 파일이 없습니다.")
            return
        filename = max(csv_files, key=os.path.getctime)
        print(f"가장 최신 파일을 분석합니다: {os.path.basename(filename)}")
    else:
        filename = os.path.join(csv_dir, filename)
    
    try:
        # 데이터 읽기
        df = pd.read_csv(filename)
        
        # 컬럼명 확인 및 정리
        print("데이터 정보:")
        print(f"총 {len(df)}개의 데이터 포인트")
        print("\n컬럼 정보:")
        print(df.columns.tolist())
        
        # 기본 통계
        numeric_columns = df.select_dtypes(include=['number']).columns
        stats_df = df[numeric_columns].describe().round(1)
        
        # 시각화
        fig, axes = plt.subplots(2, 3, figsize=(18, 10))
        fig.suptitle('TCS34725 센서 데이터 분석', fontsize=20, y=0.95)
        
        # RGB 값 시간 변화
        if 'R' in df.columns and 'G' in df.columns and 'B' in df.columns:
            axes[0, 0].plot(df.index, df['R'], 'r-', label='Red', alpha=0.7)
            axes[0, 0].plot(df.index, df['G'], 'g-', label='Green', alpha=0.7)
            axes[0, 0].plot(df.index, df['B'], 'b-', label='Blue', alpha=0.7)
            axes[0, 0].set_title('RGB 값 변화')
            axes[0, 0].set_xlabel('데이터 포인트')
            axes[0, 0].set_ylabel('RGB 값 (0-255)')
            axes[0, 0].legend()
            axes[0, 0].grid(True, alpha=0.3)
        
        # Clear 값
        if 'Clear' in df.columns:
            axes[0, 1].plot(df.index, df['Clear'], 'k-', linewidth=2)
            axes[0, 1].set_title('Clear 값 변화')
            axes[0, 1].set_xlabel('데이터 포인트')
            axes[0, 1].set_ylabel('Clear 값')
            axes[0, 1].grid(True, alpha=0.3)
        
        # Lux 값
        if 'Lux' in df.columns:
            axes[0, 2].plot(df.index, df['Lux'], 'orange', linewidth=2)
            axes[0, 2].set_title('조도(Lux) 변화')
            axes[0, 2].set_xlabel('데이터 포인트')
            axes[0, 2].set_ylabel('Lux')
            axes[0, 2].grid(True, alpha=0.3)
        
        # 색온도
        if 'Color_Temp_K' in df.columns:
            axes[1, 0].plot(df.index, df['Color_Temp_K'], 'purple', linewidth=2)
            axes[1, 0].set_title('색온도 변화')
            axes[1, 0].set_xlabel('데이터 포인트')
            axes[1, 0].set_ylabel('색온도 (K)')
            axes[1, 0].grid(True, alpha=0.3)
        
        # RGB 히스토그램
        if 'R' in df.columns and 'G' in df.columns and 'B' in df.columns:
            axes[1, 1].hist(df['R'], bins=20, alpha=0.5, color='red', label='Red')
            axes[1, 1].hist(df['G'], bins=20, alpha=0.5, color='green', label='Green')
            axes[1, 1].hist(df['B'], bins=20, alpha=0.5, color='blue', label='Blue')
            axes[1, 1].set_title('RGB 값 분포')
            axes[1, 1].set_xlabel('RGB 값')
            axes[1, 1].set_ylabel('빈도')
            axes[1, 1].legend()
            axes[1, 1].grid(True, alpha=0.3)
        
        # 기본 통계 테이블
        ax_table = axes[1, 2]
        ax_table.axis('off')
        ax_table.set_title('기본 통계 요약', y=0.9)
        
        # 표시할 통계 선택
        stats_to_show = stats_df.loc[['mean', 'std', 'min', 'max']].rename(index={'mean': '평균', 'std': '표준편차', 'min': '최소', 'max': '최대'})

        table = ax_table.table(cellText=stats_to_show.values,
                            rowLabels=stats_to_show.index,
                            colLabels=stats_to_show.columns,
                            loc='center',
                            cellLoc='center')
        table.auto_set_font_size(False)
        table.set_fontsize(8)
        table.scale(1, 1.8)
        ax_table.grid(False)
        
        # 색상 변화 감지
        analysis_text = ""
        if 'R' in df.columns and 'G' in df.columns and 'B' in df.columns:
            # ... (색상 분석 로직은 그대로)
            avg_r = df['R'].mean()
            avg_g = df['G'].mean()
            avg_b = df['B'].mean()
            
            if avg_r > avg_g and avg_r > avg_b:
                dominant_color = "빨간색"
            elif avg_g > avg_r and avg_g > avg_b:
                dominant_color = "초록색"
            elif avg_b > avg_r and avg_b > avg_g:
                dominant_color = "파란색"
            else:
                dominant_color = "중성색"
            
            rgb_std = (df['R'].std() + df['G'].std() + df['B'].std()) / 3
            
            stability = "색상 변화가 큽니다."
            if rgb_std < 10:
                stability = "색상이 매우 안정적입니다."
            elif rgb_std < 25:
                stability = "색상이 비교적 안정적입니다."

            analysis_text = (
                f"평균 RGB: ({avg_r:.1f}, {avg_g:.1f}, {avg_b:.1f})\n"
                f"색상 변화량 (표준편차 평균): {rgb_std:.1f}"
            )
            print("\n" + analysis_text)

            # 분석 텍스트를 테이블 아래에 추가
            fig.text(0.83, 0.08, analysis_text, ha="center", fontsize=12,
                    bbox={"facecolor":"white", "alpha":0.5, "pad":5})

        plt.tight_layout(rect=[0, 0, 1, 0.95]) # 레이아웃 조정
        
        # 그래프 저장
        plot_dir = os.path.join(data_dir, "plots")
        os.makedirs(plot_dir, exist_ok=True)
        base_filename = os.path.basename(filename)
        plot_filename = os.path.join(plot_dir, base_filename.replace('.csv', '_analysis.png'))
        plt.savefig(plot_filename, dpi=300, bbox_inches='tight')
        print(f"\n분석 그래프 저장: {plot_filename}")
        
        plt.show()

    except Exception as e:
        print(f"분석 중 오류 발생: {e}")

def list_data_files():
    """저장된 데이터 파일 목록 출력"""
    data_dir = "data"
    if not os.path.exists(data_dir):
        print("data 폴더가 없습니다.")
        return
    
    csv_files = glob.glob(os.path.join(data_dir, "sensor_data_*.csv"))
    if not csv_files:
        print("저장된 데이터 파일이 없습니다.")
        return
    
    print("=== 저장된 데이터 파일 목록 ===")
    for i, file in enumerate(sorted(csv_files), 1):
        filename = os.path.basename(file)
        size = os.path.getsize(file)
        mtime = datetime.fromtimestamp(os.path.getmtime(file))
        print(f"{i}. {filename} (크기: {size} bytes, 수정: {mtime.strftime('%Y-%m-%d %H:%M:%S')})")

if __name__ == "__main__":
    print("=== TCS34725 센서 데이터 분석기 ===")
    print("1. 최신 데이터 분석")
    print("2. 특정 파일 분석")
    print("3. 파일 목록 보기")
    
    choice = input("선택하세요 (1-3): ").strip()
    
    if choice == "1":
        analyze_sensor_data()
    elif choice == "2":
        list_data_files()
        filename = input("분석할 파일명을 입력하세요: ").strip()
        if filename:
            analyze_sensor_data(filename)
    elif choice == "3":
        list_data_files()
    else:
        print("기본값으로 최신 데이터를 분석합니다.")
        analyze_sensor_data()
