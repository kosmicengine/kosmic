def can_build(env, platform):
    env.module_add_dependencies("voyscript", ["jsonrpc", "websocket"], True)
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "@VoyScript",
        "VoyScript",
        "VoyScriptSyntaxHighlighter",
    ]


def get_doc_path():
    return "doc_classes"
