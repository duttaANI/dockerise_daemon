# Use an official GCC image
FROM gcc:latest

# Set the working directory
WORKDIR /app

# Copy source code
COPY daemon1.c daemon2.c ./

# Compile both programs
RUN gcc daemon1.c -o daemon1 && \
    gcc daemon2.c -o daemon2

# Default command (overridden in docker-compose)
CMD ["echo", "Built successfully!"]

