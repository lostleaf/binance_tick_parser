import pandas as pd


def convert_to_dataframe(data):
    d = dict()
    for field, ftype in data.dtype.fields.items():
        ftype = ftype[0]
        if ftype.subdtype is None:
            d[field] = data[field]
        else:
            for i in range(ftype.subdtype[1][0]):
                d[f'{field}{i + 1}'] = data[field][:, i]
    df = pd.DataFrame(d)
    df['timestamp'] = pd.to_datetime(df['timestamp'], unit='ms', utc=True)
    df.set_index('timestamp', inplace=True)
    return df