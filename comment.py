def generate_function_comment():
    # Ask for function name
    function_name = input("Function Name: ").strip()

    # Ask for explanation
    explanation = input("Explanation: ").strip()

    # Ask for parameters until the user stops
    parameters = []
    while True:
        parameter_name = input("Parameter Name (leave empty to stop): ").strip()
        if not parameter_name:
            break
        parameter_type = input(f"Parameter Type for {parameter_name}: ").strip()
        parameter_direction = input(f"Parameter Direction for {parameter_name} (In/Out/In/Out): ").strip()
        parameters.append((parameter_name, parameter_type, parameter_direction))

    # Ask for return value
    return_value = input("Return Value: ").strip()

    # Generate the comment
    comment = f"//----------------------------------------------------------------------------------------\n"
    comment += f"//                                 {function_name}\n"
    comment += f"//                                 {'-' * len(function_name)}\n"
    comment += f"//\n"
    comment += f"// General      : {explanation}\n"
    comment += f"//\n"
    if parameters:
        comment += f"// Parameters   :\n"
        for param_name, param_type, param_direction in parameters:
            comment += f"//          {param_name:<10} - {param_type} ({param_direction})\n"
    comment += f"//\n"
    comment += f"// Return Value : {return_value}\n"
    comment += f"//\n"
    comment += f"//----------------------------------------------------------------------------------------"

    return comment

# Example usage:
if __name__ == "__main__":
    while True:
        comment = generate_function_comment()
        print(comment)
