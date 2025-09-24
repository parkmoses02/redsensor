import serial
import csv
import datetime
import time
import os

def collect_sensor_data(port='COM6', baudrate=9600, duration=30, filename=None):
    """
    아두이노에서 센서 데이터를 수집하여 CSV 파일로 저장
    
    Args:
        port (str): 시리얼 포트 (기본값: COM6)
        baudrate (int): 보드레이트 (기본값: 9600)
        duration (int): 수집 시간(초) (기본값: 30초, 0이면 무한히 수집)
        filename (str): 저장할 파일명 (기본값: 현재 시간 기준 자동 생성)
    """
    
    # 파일명이 지정되지 않으면 현재 시간으로 생성
    if filename is None:
        timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        filename = f"sensor_data_{timestamp}.csv"
    
    # data 폴더 및 하위 폴더 생성
    data_dir = "data"
    csv_dir = os.path.join(data_dir, "csv_files")
    os.makedirs(csv_dir, exist_ok=True)
    
    filepath = os.path.join(csv_dir, filename)
    
    try:
        # 시리얼 포트 연결
        ser = serial.Serial(port, baudrate, timeout=1)
        print(f"시리얼 포트 {port} 연결 성공")
        
        # 포트 안정화를 위해 잠시 대기
        time.sleep(2)
        
        # 기존 버퍼 비우기
        ser.flushInput()
        
        # CSV 파일 준비
        with open(filepath, 'w', newline='', encoding='utf-8') as csvfile:
            writer = csv.writer(csvfile)
            
            # 헤더 미리 작성 (타임스탬프 제거)
            writer.writerow(['R', 'G', 'B', 'Clear', 'Lux', 'Color_Temp_K'])
            header_written = True
            start_time = time.time()
            data_count = 0
            
            print(f"데이터 수집 시작... (파일: {filepath})")
            if duration > 0:
                print(f"수집 시간: {duration}초")
            else:
                print("무한 수집 모드 (Ctrl+C로 중지)")
            
            while True:
                try:
                    # 종료 조건 확인
                    if duration > 0 and (time.time() - start_time) > duration:
                        break
                    
                    # 시리얼 데이터 읽기
                    if ser.in_waiting > 0:
                        line = ser.readline().decode('utf-8').strip()
                        
                        if line:
                            print(f"수신: {line}")
                            
                            # 데이터 처리 (숫자로만 구성된 행)
                            if ',' in line:
                                try:
                                    data = line.split(',')
                                    # 모든 값이 숫자인지 확인 (6개 값: R,G,B,Clear,Lux,Color_Temp)
                                    if len(data) == 6:
                                        [float(x) for x in data]
                                        
                                        # 타임스탬프 없이 센서 데이터만 저장
                                        writer.writerow(data)
                                        csvfile.flush()  # 즉시 파일에 쓰기
                                        
                                        data_count += 1
                                        print(f"데이터 저장: #{data_count} - {line}")
                                        
                                except ValueError:
                                    # 숫자가 아닌 데이터는 무시
                                    continue
                
                except KeyboardInterrupt:
                    print("\n사용자에 의해 중지됨")
                    break
                    
                time.sleep(0.1)  # CPU 사용량 조절
        
        print(f"\n데이터 수집 완료!")
        print(f"총 {data_count}개의 데이터 저장됨")
        print(f"파일 위치: {filepath}")
        
    except serial.SerialException as e:
        print(f"시리얼 포트 오류: {e}")
        print("아두이노가 연결되어 있고 포트가 올바른지 확인하세요.")
    except Exception as e:
        print(f"오류 발생: {e}")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("시리얼 포트 연결 종료")

if __name__ == "__main__":
    # 사용 예시
    print("=== TCS34725 센서 데이터 수집기 ===")
    
    # 파일명 입력 받기
    custom_filename = input("저장할 파일명을 입력하세요 (확장자 없이, 비워두면 자동 생성): ").strip()
    if custom_filename:
        filename = f"{custom_filename}.csv"
    else:
        filename = None

    print("1. 30초 동안 수집")
    print("2. 사용자 정의 시간")
    print("3. 무한 수집 (Ctrl+C로 중지)")
    
    choice = input("선택하세요 (1-3): ").strip()
    
    if choice == "1":
        collect_sensor_data(duration=30, filename=filename)
    elif choice == "2":
        try:
            duration = int(input("수집 시간(초)을 입력하세요: "))
            collect_sensor_data(duration=duration, filename=filename)
        except ValueError:
            print("올바른 숫자를 입력하세요.")
    elif choice == "3":
        collect_sensor_data(duration=0, filename=filename)
    else:
        print("기본값으로 30초 수집을 시작합니다.")
        collect_sensor_data(duration=30, filename=filename)
