```bash
conda activate lab11env
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
export SPARK_HOME=~/spark-1.1.0
export PYTHONPATH=$SPARK_HOME/python:$SPARK_HOME/python/lib/py4j-0.8.2.1-src.zip:$PYTHONPATH
export PATH=$PATH:$SPARK_HOME/bin
```