import java.io.IOException;
import java.net.UnknownHostException;
import java.util.List;
import java.util.ArrayList;

import netapp.manage.NaElement;
import netapp.manage.NaException;
import netapp.manage.NaServer;

import com.google.common.base.Objects;

public class VolumeList {
    private class NetAppVolumeCmode {

        public String name;
        public String fsid;
        public String instanceUuid;
        public String uuid;
        public String junctionParent;
        public String junctionPath;
        public String owningVserverName;
        public String owningVserverUuid;
        public String policy;

        public String toString() {
           return Objects.toStringHelper(this.getClass())
           .add("name", name)
           .add("fsid", fsid)
           .add("instanceUuid", instanceUuid)
           .add("uuid", uuid)
           .add("junctionParent", junctionParent)
           .add("junctionPath", junctionPath)
           .add("owningVserverName", owningVserverName)
           .add("owningVserverPath", owningVserverUuid)
           .add("policy", policy).toString();
        }
    };
    

    public static void main(String[] args) {
        try {
            NaServer s = new NaServer("192.168.29.34");
            s.setServerType(NaServer.SERVER_TYPE_FILER);
            s.setTransportType(NaServer.TRANSPORT_TYPE_HTTPS);
            s.setPort(443);
            s.setStyle(NaServer.STYLE_LOGIN_PASSWORD);
            s.setAdminUser("vsadmin", "12!pass345");

/*
            // CIFS export
            NaElement api = new NaElement("cifs-share-create");
            api.addNewChild("path", "/vsrv1_vol1/.snapshot/testsnap1/a/aa");
            api.addNewChild("share-name","test_snapshare1");
*/
            List<NetAppVolumeCmode> volList = null;
            String tag = null;
            while (true) {
                NaElement input = new NaElement("volume-get-iter");
                NaElement desiredAttrs = new NaElement("desired-attributes");
                NaElement volumeAttrs = new NaElement("volume-attributes");

                NaElement exportAttrs = new NaElement("volume-export-attributes");
                exportAttrs.addChildElem(new NaElement("policy"));

                NaElement idAttrs = new NaElement("volume-id-attributes");
                idAttrs.addChildElem(new NaElement("fsid"));
                idAttrs.addChildElem(new NaElement("instance-uuid"));
                idAttrs.addChildElem(new NaElement("junction-parent-name"));
                idAttrs.addChildElem(new NaElement("junction-path"));
                idAttrs.addChildElem(new NaElement("name"));
                idAttrs.addChildElem(new NaElement("owning-vserver-name"));
                idAttrs.addChildElem(new NaElement("owning-vserver-uuid"));
                idAttrs.addChildElem(new NaElement("uuid"));

                volumeAttrs.addChildElem(idAttrs);
                volumeAttrs.addChildElem(exportAttrs);
                desiredAttrs.addChildElem(volumeAttrs);
                input.addChildElem(desiredAttrs);

                input.addNewChild("max-records", "1024");
                if (tag != null)
                    input.addNewChild("tag", tag);

                NaElement iterOut = s.invokeElem(input);
                //System.out.println(iterOut);

                NaElement count = iterOut.getChildByName("num-records");
                if (count.getContent().equals("0")) {
                    break;
                }

                VolumeList list = new VolumeList();
                volList = list.parseVolumeList(iterOut);

                NaElement tagElem = iterOut.getChildByName("next-tag");
                if (tagElem != null)
                    tag = iterOut.getChildByName("next-tag").getContent();

                if (tag == null)
                    break;
            }

            if (volList != null) {
                for (int i = 0; i < volList.size(); i++) {
                    NetAppVolumeCmode vol = volList.get(i);
                    System.out.println(vol);
                }
            }

        } catch (NaException e) {
            handleException(e);
        } catch (UnknownHostException e) {
            handleException(e);
        } catch (IOException e) {
            handleException(e);
        }
    }

    private static void handleException(Exception e) {
        System.out.println(e.getMessage());
        e.printStackTrace();        
    }

    private List<NetAppVolumeCmode> parseVolumeList(NaElement out) {
        List<NetAppVolumeCmode> volList = new ArrayList<NetAppVolumeCmode>();
        List<?> objects = out.getChildByName("attributes-list").getChildren();
        for (Object obj : objects) {
            NaElement rule = (NaElement) obj;
            System.out.println("parseVolumeList name " + rule.getName());
            if ("volume-attributes".equals(rule.getName())) {
                NetAppVolumeCmode vol = parseVolumeAttrs(rule);
                volList.add(vol);
            }
        }

        return volList;
    }

    private NetAppVolumeCmode parseVolumeAttrs(NaElement out) {
        NetAppVolumeCmode vol = new NetAppVolumeCmode();
        List<?> objects = out.getChildren();
        for (Object obj : objects) {
            NaElement rule = (NaElement) obj;
            System.out.println("parseVolumeAttrs name " + rule.getName());
            if ("volume-id-attributes".equals(rule.getName())) {
                parseVolumeId(rule, vol);
                continue;
            }
            if ("volume-export-attributes".equals(rule.getName())) {
                parseVolumeExport(rule, vol);
                continue;
            }
        }

        return vol;
    }

    private void parseVolumeId(NaElement out, NetAppVolumeCmode vol) {
        NaElement name = out.getChildByName("name");
        vol.name = name != null ? name.getContent() : "";
        NaElement fsid = out.getChildByName("fsid");
        vol.fsid = fsid != null ? fsid.getContent() : "";
        NaElement instanceUuid = out.getChildByName("instance-uuid");
        vol.instanceUuid = instanceUuid != null ? instanceUuid.getContent() : "";
        NaElement uuid = out.getChildByName("uuid");
        vol.uuid = uuid != null ? uuid.getContent() : "";
        NaElement junctionParent = out.getChildByName("junction-parent-name");
        vol.junctionParent = junctionParent != null ? junctionParent.getContent() : "";
        NaElement junctionPath = out.getChildByName("junction-path");
        vol.junctionPath = junctionPath != null ? junctionPath.getContent() : "";
        NaElement owningVserverName = out.getChildByName("owning-vserver-name");
        vol.owningVserverName = owningVserverName != null ? owningVserverName.getContent() : "";
        NaElement owningVserverUuid = out.getChildByName("owning-vserver-uuid");
        vol.owningVserverUuid = owningVserverUuid != null ? owningVserverUuid.getContent() : "";
    }

    private void parseVolumeExport(NaElement out, NetAppVolumeCmode vol) {
        NaElement policy = out.getChildByName("policy");
        vol.policy = policy != null ? policy.getContent() : "";
    }
}

