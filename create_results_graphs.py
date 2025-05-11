import matplotlib.pyplot as plt

files = ["results_logic_text.txt","results_repeat_text_alf1.txt","results_repeat_text_alf5.txt","results_repeat_text_alf10.txt","results_repeat_text_alf15.txt","results_repeat_text_alf20.txt","results_repeat_text_alf26.txt","results_repeat_text_alf59.txt","results_repeat_text_alf118.txt", "results_repeat_text_alf158.txt"]
dir="results/"
files=[dir+file for file in files]

for file in files:

    text={
        "num": [],
        "size": []
    }
    lz77={
        "t_arch": [],
        "t_unz": [],
        "memory": [],
        "coef": []
    }
    lz78= {
        "t_arch": [],
        "t_unz": [],
        "memory": [],
        "coef": []
    }
    huffman= {
        "t_arch": [],
        "t_unz": [],
        "memory": [],
        "coef": []
    }
    deflate= {
        "t_arch": [],
        "t_unz": [],
        "memory": [],
        "coef": []
    }

    with open(file, 'r') as results_logic_text:
        for line in results_logic_text:
            temp_line=line.strip().split()
            temp_line=[item for item in temp_line if len(item)!=0]
            text["num"].append(int(temp_line[0]))
            text["size"].append(int(temp_line[1]))
            huffman["t_arch"].append(float(temp_line[2]))
            huffman["t_unz"].append(float(temp_line[3]))
            huffman["memory"].append(float(temp_line[4]))
            huffman["coef"].append(float(temp_line[5]))
            lz77["t_arch"].append(float(temp_line[6]))
            lz77["t_unz"].append(float(temp_line[7]))
            lz77["memory"].append(float(temp_line[8]))
            lz77["coef"].append(float(temp_line[9]))
            lz78["t_arch"].append(float(temp_line[10]))
            lz78["t_unz"].append(float(temp_line[11]))
            lz78["memory"].append(float(temp_line[12]))
            lz78["coef"].append(float(temp_line[13]))
            deflate["t_arch"].append(float(temp_line[14]))
            deflate["t_unz"].append(float(temp_line[15]))
            deflate["memory"].append(float(temp_line[16]))
            deflate["coef"].append(float(temp_line[17]))

    def show_graph(ax, key, *, title="Undefined", xlabel="Undefined", ylabel="Undefined", log_scale=False):
        ax.plot(text["size"], huffman[key], label="HUFFMAN", color='blue', marker='o', linestyle='-')
        ax.plot(text["size"], lz77[key], label="LZ77", color='green', marker='s', linestyle='--')
        ax.plot(text["size"], lz78[key], label="LZ78", color='red', marker='^', linestyle=':')
        ax.plot(text["size"], deflate[key], label="DEFLATE", color='purple', marker='D', linestyle='-.')

        if log_scale:
            ax.set_yscale('log')

        ax.set_title(title, fontsize=12)
        ax.set_xlabel(xlabel, fontsize=10)
        ax.set_ylabel(ylabel, fontsize=10)
        ax.grid(True, linestyle='--', alpha=0.7)
        ax.legend(loc='best', fontsize=8)
        ax.tick_params(axis='x', rotation=45)

    fig, axs = plt.subplots(nrows=2, ncols=2, figsize=(10, 8))
    fig.canvas.manager.set_window_title(results_logic_text.name)
    fig.suptitle(f"Графики алгоритмов сжатия на основе {results_logic_text.name}", fontsize=14, y=0.98)

    show_graph(axs[0, 0], "t_arch", title="Время архивации", xlabel="Размер файла (КБ)", ylabel="Время (сек)")
    show_graph(axs[0, 1], "t_unz", title="Время распаковки", xlabel="Размер файла (КБ)", ylabel="Время (сек)")
    show_graph(axs[1, 0], "memory", title="Использование памяти", xlabel="Размер файла (КБ)", ylabel="Память (байт)", log_scale=True)
    show_graph(axs[1, 1], "coef", title="Коэффициент сжатия", xlabel="Размер файла (КБ)", ylabel="Коэффициент")

    plt.tight_layout()
    plt.show()